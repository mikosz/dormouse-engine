#include "RenderingFixture.hpp"

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
}
