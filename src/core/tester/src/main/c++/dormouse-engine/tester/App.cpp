#include "App.hpp"

#include <iostream>
#include <memory>
#include <string>

#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_MODULE "dormouse_engine::tester"
#include <boost/test/included/unit_test.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/filesystem.hpp>

#include "dormouse-engine/essentials/Range.hpp"
#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/essentials/test-utils/test-utils.hpp"
#include "dormouse-engine/system/platform.hpp"
#include "dormouse-engine/logger/macros.hpp"
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

DE_LOGGER_CATEGORY("DORMOUSE_ENGINE.TESTER.APP");

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

class LoggerSink : public boost::iostreams::sink {
public:

	std::streamsize write(const char* str, std::streamsize n) {
		buffer_ += std::string_view(str, n);
		for (;;) {
			const auto newlinePos = buffer_.find('\n');
			if (newlinePos != std::string::npos) {
				DE_LOG_INFO << buffer_.substr(0u, newlinePos);
				buffer_.erase(0u, newlinePos + 1);
			} else {
				break;
			}
		}
		return n;
	}

private:

	std::shared_ptr<logger::Logger> logger_;

	std::string buffer_;

};

class GlobalTestLoggerStream :
	public essentials::Singleton<
		GlobalTestLoggerStream,
		essentials::policy::creation::New<GlobalTestLoggerStream>
		>
{
public:

	GlobalTestLoggerStream() noexcept :
		stream_(sink_)
	{
	}

	~GlobalTestLoggerStream() {
		stream_.flush();
		boost::unit_test::unit_test_log.set_stream(std::cout);
	}

	void bindLogger() noexcept {
		boost::unit_test::unit_test_log.set_stream(stream_);
		boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_successful_tests);
		boost::unit_test::unit_test_log.set_format(boost::unit_test::OF_CLF);
	}

private:

	LoggerSink sink_;

	boost::iostreams::stream<LoggerSink> stream_;

};

boost::unit_test::test_suite* initUnitTest(int, char**) {
	GlobalTestLoggerStream::instance()->bindLogger();
	return nullptr;
}

graphics::Texture createScreenshotGPUTexture(graphics::Device& graphicsDevice, size_t width, size_t height) {
	auto configuration = graphics::Texture::Configuration2d();
	configuration.allowCPUWrite = false;
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = true;
	configuration.width = width;
	configuration.height = height;
	configuration.purposeFlags = graphics::Texture::Purpose::SHADER_RESOURCE;
	configuration.pixelFormat = graphics::FORMAT_R8G8B8A8_UNORM; // TODO

	return graphics::Texture(graphicsDevice, configuration);
}

graphics::Texture createScreenshotCPUTexture(graphics::Device& graphicsDevice, size_t width, size_t height) {
	auto configuration = graphics::Texture::Configuration2d();
	configuration.allowCPUWrite = false;
	configuration.allowCPURead = true;
	configuration.allowGPUWrite = true;
	configuration.width = width;
	configuration.height = height;
	configuration.pixelFormat = graphics::FORMAT_R8G8B8A8_UNORM; // TODO

	return graphics::Texture(graphicsDevice, configuration);
}

} // anonymous namespace

App::App() :
	engineApp_(*wm::GlobalMainArguments::instance(), windowConfiguration()),
	textureComparator_(graphicsDevice()),
	screenshotGPU_(
		createScreenshotGPUTexture(
			graphicsDevice(), engineApp_.mainWindow().clientWidth(), engineApp_.mainWindow().clientHeight()
			)
		),
	screenshotCPU_(
		createScreenshotCPUTexture(
			graphicsDevice(), engineApp_.mainWindow().clientWidth(), engineApp_.mainWindow().clientHeight()
			)
		)
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

	auto screenshotRowPitch = size_t();

	const auto pixelFormat = graphics::FORMAT_R8G8B8A8_UNORM; // TODO
	const auto width = engineApp_.mainWindow().clientWidth();
	const auto height = engineApp_.mainWindow().clientHeight();

	commandList.copy(graphicsDevice().backBuffer(), screenshotGPU_);

	const auto currentTestCaseName = boost::unit_test::framework::current_test_case().p_name.get();

	const auto referencePath =
		boost::filesystem::path("test/reference") /
		(currentTestCaseName + "." + std::to_string(index) + ".tga")
		;

	if (boost::filesystem::exists(referencePath)) {
		const auto referenceImageData = essentials::test_utils::readBinaryFile(referencePath);
		const auto referenceImage = graphics::Image::load(
			essentials::viewBuffer(referenceImageData), referencePath);
		const auto referenceTexture = graphics::Texture(graphicsDevice(), referenceImage);

		auto same = textureComparator_.compare(
			graphicsDevice(),
			referenceTexture,
			screenshotGPU_,
			width,
			height
			);

		BOOST_CHECK(same);

		if (!same) {
			auto screenshotPixels = essentials::ByteVector();

			commandList.copy(screenshotGPU_, screenshotCPU_);

			auto lockedScreenshotData = commandList.lock(screenshotCPU_, graphics::CommandList::LockPurpose::READ);
			screenshotRowPitch = lockedScreenshotData.rowPitch;

			screenshotPixels.resize(height * screenshotRowPitch);
			std::copy(
				lockedScreenshotData.pixels.get(),
				lockedScreenshotData.pixels.get() + screenshotPixels.size(),
				screenshotPixels.data()
				);

			auto screenshotImage = graphics::Image(
				std::move(screenshotPixels),
				std::make_pair(width, height),
				1u,
				1u,
				pixelFormat
				);

			const auto candidatePath = referencePath.string() + ".bad";

			screenshotImage.save(candidatePath, screenshotRowPitch);

			BOOST_CHECK_MESSAGE(
				false,
				"Reference image for test " +
				currentTestCaseName +
				" (" +
				referencePath.string() +
				" different. Bad screenshot stored at " +
				candidatePath
				);
		}
	} else {
		// TODO: duplicated
		auto screenshotPixels = essentials::ByteVector();

		commandList.copy(screenshotGPU_, screenshotCPU_);

		auto lockedScreenshotData = commandList.lock(screenshotCPU_, graphics::CommandList::LockPurpose::READ);
		screenshotRowPitch = lockedScreenshotData.rowPitch;

		screenshotPixels.resize(height * screenshotRowPitch);
		std::copy(
			lockedScreenshotData.pixels.get(),
			lockedScreenshotData.pixels.get() + screenshotPixels.size(),
			screenshotPixels.data()
			);

		auto screenshotImage = graphics::Image(
			std::move(screenshotPixels),
			std::make_pair(width, height),
			1u,
			1u,
			pixelFormat
			);

		const auto candidatePath = referencePath.string() + ".candidate";

		screenshotImage.save(candidatePath, screenshotRowPitch);

		BOOST_CHECK_MESSAGE(
			false,
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
	try {
		int result = boost::unit_test::unit_test_main(&initUnitTest, __argc, __argv);
		return result;
	} catch (...) {
		return 1;
	}
}
