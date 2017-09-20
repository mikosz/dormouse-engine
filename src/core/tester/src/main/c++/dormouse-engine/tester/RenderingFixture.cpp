#include "RenderingFixture.hpp"

#include "dormouse-engine/essentials/test-utils/test-utils.hpp"
#include "dormouse-engine/essentials/memory.hpp"
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
