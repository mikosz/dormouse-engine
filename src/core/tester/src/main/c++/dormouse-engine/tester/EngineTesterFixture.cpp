#include "EngineTesterFixture.hpp"

#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/framework.hpp>

#include "dormouse-engine/essentials/Range.hpp"
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



} // anonymous namespace

EngineTesterFixture::EngineTesterFixture() :
	graphicsDevice_(window().handle(), graphicsDeviceConfiguration()),
	
{
}
