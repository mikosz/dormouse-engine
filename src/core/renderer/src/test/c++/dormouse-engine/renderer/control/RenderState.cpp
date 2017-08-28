#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/renderer/control/RenderState.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::control;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(RenderStateFactoryTestSuite);

BOOST_AUTO_TEST_CASE(CreatesRenderStates) {
	auto graphicsDevice = graphics::Device();

	auto configuration = graphics::RenderState::Configuration();
	configuration.frontCounterClockwise = true;
	configuration.blendingEnabled = false;
	configuration.cullMode = graphics::RenderState::CullMode::FRONT;
	configuration.fillMode = graphics::RenderState::FillMode::WIREFRAME;

	RenderStateFactory::setInstance(nullptr);
	auto renderState = RenderStateFactory::instance()->create(configuration);

	auto resultConfiguration = renderState.configuration();

	BOOST_CHECK_EQUAL(resultConfiguration.frontCounterClockwise, configuration.frontCounterClockwise);
	BOOST_CHECK_EQUAL(resultConfiguration.blendingEnabled, configuration.blendingEnabled);
	BOOST_CHECK_EQUAL(resultConfiguration.cullMode, configuration.cullMode);
	BOOST_CHECK_EQUAL(resultConfiguration.fillMode, configuration.fillMode);
}

BOOST_AUTO_TEST_CASE(ReturnsEqualRenderStatesForSameConfiguration) {
	auto configuration = graphics::RenderState::Configuration();
	configuration.frontCounterClockwise = true;
	configuration.blendingEnabled = false;
	configuration.cullMode = graphics::RenderState::CullMode::FRONT;
	configuration.fillMode = graphics::RenderState::FillMode::WIREFRAME;

	RenderStateFactory::setInstance(nullptr);
	auto renderStateL = RenderStateFactory::instance()->create(configuration);
	auto renderStateR = RenderStateFactory::instance()->create(configuration);

	BOOST_CHECK(renderStateL == renderStateR);
}

BOOST_AUTO_TEST_CASE(ReturnsUnequalRenderStatesForDifferentConfiguration) {
	auto configuration = graphics::RenderState::Configuration();
	configuration.frontCounterClockwise = true;
	configuration.blendingEnabled = false;
	configuration.cullMode = graphics::RenderState::CullMode::FRONT;
	configuration.fillMode = graphics::RenderState::FillMode::WIREFRAME;

	RenderStateFactory::setInstance(nullptr);
	auto renderStateL = RenderStateFactory::instance()->create(configuration);

	configuration.blendingEnabled = !configuration.blendingEnabled;

	auto renderStateR = RenderStateFactory::instance()->create(configuration);

	BOOST_CHECK(renderStateL != renderStateR);
}

BOOST_AUTO_TEST_SUITE_END(/* RenderStateFactoryTestSuite */);

} // anonymous namespace
