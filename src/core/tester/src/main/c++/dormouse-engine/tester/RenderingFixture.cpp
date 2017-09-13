#include "RenderingFixture.hpp"

#include "dormouse-engine/essentials/test-utils/test-utils.hpp"
#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/renderer/d2/Sprite.hpp"
#include "dormouse-engine/renderer/control/RenderState.hpp"
#include "dormouse-engine/renderer/control/ResourceView.hpp"
#include "dormouse-engine/renderer/control/Sampler.hpp"

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

} // anonymous namespace

RenderingFixture::RenderingFixture() :
	graphicsDevice_(window().handle(), graphicsDeviceConfiguration())
{
	renderer::d2::Sprite::initialiseSystem(
		graphicsDevice_,
		essentials::viewBuffer(essentials::test_utils::readFile("sprite.hlsl"))
		);

	renderer::control::RenderState::initialiseSystem(graphicsDevice_);
	renderer::control::ResourceView::initialiseSystem(graphicsDevice_);
	renderer::control::Sampler::initialiseSystem(graphicsDevice_);
}
