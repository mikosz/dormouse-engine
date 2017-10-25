#include "App.hpp"

#include <iostream>
#include <memory>

#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_MODULE "dormouse_engine::tester"
#include <boost/test/included/unit_test.hpp>

#include <boost/filesystem.hpp>

#include "dormouse-engine/essentials/Range.hpp"
#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/essentials/test-utils/test-utils.hpp"
#include "dormouse-engine/system/platform.hpp"
#include "dormouse-engine/logger/Logger.hpp"
#include "dormouse-engine/logger/LoggerStringbuf.hpp"
#include "dormouse-engine/logger/appender/DebugWindowAppender.hpp"
#include "dormouse-engine/logger/layout/BasicLayout.hpp"
#include "dormouse-engine/graphics/Image.hpp"
#include "dormouse-engine/graphics/Texture.hpp"
#include "dormouse-engine/renderer/d2/Sprite.hpp"
#include "dormouse-engine/renderer/control/RenderState.hpp"
#include "dormouse-engine/renderer/control/ResourceView.hpp"
#include "dormouse-engine/renderer/control/Sampler.hpp"
#include "dormouse-engine/renderer/control/RenderTargetView.hpp"
#include "dormouse-engine/renderer/control/DepthStencilView.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::tester;

namespace /* anonymous */ {

dormouse_engine::wm::Window::Configuration windowConfiguration() {
	using namespace dormouse_engine;

	auto result = wm::Window::Configuration();

	result.className = "DormouseEngineTesterWindow";
	result.title = "dormouse_engine::tester";
	result.fullscreen = false;
	result.width = 800u;
	result.height = 600u;

	return result;
}

renderer::control::Viewport createViewport(const wm::Window& window) {
	auto configuration = graphics::Viewport::Configuration();
	configuration.height = static_cast<float>(window.clientHeight());
	configuration.width = static_cast<float>(window.clientWidth());
	configuration.minDepth = 0.0f;
	configuration.maxDepth = 1.0f;
	configuration.topLeftX = 0.0f;
	configuration.topLeftY = 0.0f;
	return renderer::control::Viewport(configuration);
}

// TODO: just change reporter in boost test
void redirectCErrToLogger() {
#if defined(DE_COMPILER_VISUAL_CXX)
	auto boostTestLogger = std::make_shared<logger::Logger>(logger::Level::INFO);
	auto boostTestLayout = std::make_shared<logger::layout::BasicLayout>();
	auto boostTestAppender = std::make_shared<logger::appender::DebugWindowAppender>(
		logger::Level::INFO, std::move(boostTestLayout));

	boostTestLogger->addAppender(std::move(boostTestAppender));

	//boost::unit_test::unit_test_log.set_stream();
#endif /* DE_COMPILER_VISUAL_CXX */
}

boost::unit_test::test_suite* initUnitTest(int, char**) {
	return nullptr;
}

} // anonymous namespace

App::App() :
	engineApp_(*wm::GlobalMainArguments::instance(), windowConfiguration()),
	fullscreenViewport_(createViewport(engineApp_.mainWindow()))
{
	renderer::d2::Sprite::initialiseSystem(
		graphicsDevice(),
		essentials::viewBuffer(essentials::test_utils::readFile("sprite.hlsl"))
		);

	renderer::control::RenderState::initialiseSystem(graphicsDevice());
	renderer::control::ResourceView::initialiseSystem(graphicsDevice());
	renderer::control::Sampler::initialiseSystem(graphicsDevice());
	renderer::control::RenderTargetView::initialiseSystem(graphicsDevice());
	renderer::control::DepthStencilView::initialiseSystem(graphicsDevice());
}

void App::frame() {
	engineApp_.frame();
}

void App::compareWithReferenceScreen(size_t index) {
	auto& commandList = graphicsDevice().getImmediateCommandList();

	auto screenshotPixels = essentials::ByteVector();
	auto screenshotRowPitch = size_t();

	const auto pixelFormat = graphics::FORMAT_R8G8B8A8_UNORM; // TODO
	const auto width = engineApp_.mainWindow().clientWidth();
	const auto height = engineApp_.mainWindow().clientHeight();

	{
		auto configuration = graphics::Texture::Configuration2d();
		configuration.allowCPURead = true;
		configuration.allowGPUWrite = true;
		configuration.allowModifications = true;
		configuration.width = width;
		configuration.height = height;
		configuration.pixelFormat = pixelFormat;

		auto screenshot = graphics::Texture(graphicsDevice(), configuration);

		commandList.copy(graphicsDevice().backBuffer(), screenshot);

		auto lockedScreenshotData = commandList.lock(screenshot, graphics::CommandList::LockPurpose::READ);
		screenshotRowPitch = lockedScreenshotData.rowPitch;

		screenshotPixels.resize(configuration.height * screenshotRowPitch);
		std::copy(
			lockedScreenshotData.pixels.get(),
			lockedScreenshotData.pixels.get() + screenshotPixels.size(),
			screenshotPixels.data()
		);
	}

	const auto currentTestCaseName = boost::unit_test::framework::current_test_case().p_name.get();

	const auto referencePath =
		boost::filesystem::path("test/reference") /
		(currentTestCaseName + "." + std::to_string(index) + ".tga")
		;

	if (boost::filesystem::exists(referencePath)) {
		// TODO: do this on the GPU, will be wayyy faster and wayyyyyyy cooler, also would avoid all
		// this hacking with row pitches etc
		const auto referenceImageData = essentials::test_utils::readBinaryFile(referencePath);
		const auto referenceImage = graphics::Image::load(
			essentials::viewBuffer(referenceImageData), referencePath);

		const auto referencePixels = referenceImage.pixels();

		const auto minBufferSize = (height - 1) * pixelFormat.rowPitch(width) + width;
		BOOST_REQUIRE(screenshotPixels.size() >= minBufferSize);
		BOOST_REQUIRE(referencePixels.size() >= minBufferSize);

		auto different = false;

		for (const auto rowIdx : essentials::IndexRange(0u, height)) {
			const auto referenceRowOffset = rowIdx * pixelFormat.rowPitch(width);
			const auto screenshotRowOffset = rowIdx * screenshotRowPitch;

			for (const auto byteIdx : essentials::IndexRange(0u, width * pixelFormat.pixelSize())) {
				const auto referenceByteOffset = referenceRowOffset + byteIdx;
				const auto screenshotByteOffset = screenshotRowOffset + byteIdx;
				if (referencePixels.data()[referenceByteOffset] != screenshotPixels[screenshotByteOffset]) {
					different = true;
					break;
				}
			}

			if (different) {
				break;
			}
		}

		if (different) {
			auto screenshotImage = graphics::Image(
				std::move(screenshotPixels),
				std::make_pair(width, height),
				1u,
				1u,
				pixelFormat
			);

			const auto candidatePath = referencePath.string() + ".bad";

			screenshotImage.save(candidatePath, screenshotRowPitch);

			BOOST_FAIL(
				"Reference image for test " +
				currentTestCaseName +
				" (" +
				referencePath.string() +
				" different. Bad screenshot stored at " +
				candidatePath
			);
		}
	} else {
		auto screenshotImage = graphics::Image(
			std::move(screenshotPixels),
			std::make_pair(width, height),
			1u,
			1u,
			pixelFormat
		);

		const auto candidatePath = referencePath.string() + ".candidate";

		screenshotImage.save(candidatePath, screenshotRowPitch);

		BOOST_FAIL(
			"Reference image for test " +
			currentTestCaseName +
			" (" +
			referencePath.string() +
			" not found. Candidate stored at " +
			candidatePath
		);
	}
}

// --- main

DE_APP_MAIN()
	redirectCErrToLogger();

	return boost::unit_test::unit_test_main(&initUnitTest, __argc, __argv);
}
