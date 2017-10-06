#include "RenderingFixture.hpp"

#include <boost/test/framework.hpp>

#include "dormouse-engine/essentials/test-utils/test-utils.hpp"
#include "dormouse-engine/essentials/memory.hpp"
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

graphics::Device::Configuration graphicsDeviceConfiguration() {
	auto result = graphics::Device::Configuration();

	result.debugDevice = true;
	result.fullscreen = false;
	result.sampleCount = 1u;
	result.sampleQuality = 0u;
	result.vsync = false;

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

} // anonymous namespace

RenderingFixture::RenderingFixture() :
	graphicsDevice_(window().handle(), graphicsDeviceConfiguration()),
	fullscreenViewport_(createViewport(window()))
{
	renderer::d2::Sprite::initialiseSystem(
		graphicsDevice_,
		essentials::viewBuffer(essentials::test_utils::readFile("sprite.hlsl"))
		);

	renderer::control::RenderState::initialiseSystem(graphicsDevice_);
	renderer::control::ResourceView::initialiseSystem(graphicsDevice_);
	renderer::control::Sampler::initialiseSystem(graphicsDevice_);
	renderer::control::RenderTargetView::initialiseSystem(graphicsDevice_);
	renderer::control::DepthStencilView::initialiseSystem(graphicsDevice_);
}

void RenderingFixture::compareWithReferenceScreen(size_t index) const {
	auto configuration = graphics::Texture::Configuration2d();
	configuration.allowCPURead = true;
	configuration.allowGPUWrite = true;
	configuration.allowModifications = true;
	configuration.width = window().clientWidth();
	configuration.height = window().clientWidth();
	configuration.pixelFormat = graphics::FORMAT_R8G8B8A8_UNORM;

	auto screenshot = graphics::Texture(graphicsDevice_, configuration);

	graphicsDevice_.getImmediateCommandList().copy(graphicsDevice_.backBuffer(), screenshot);

	const auto referencePath =
		boost::filesystem::path("test/") /
		boost::unit_test::framework::current_test_case().p_name.get() /
		("screenshot_" + std::to_string(index) + ".png")
		;

	const auto pixels = test_utils::readBinaryFile(referencePath);
	const auto image = graphics::Image::load(essentials::viewBuffer(pixels), referencePath);

	
}
