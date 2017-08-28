#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/renderer/control/RenderState.hpp"
#include "dormouse-engine/tester/RenderingFixture.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::control;

namespace /* anonymous */ {

BOOST_FIXTURE_TEST_SUITE(RenderStateFactoryTestSuite, tester::RenderingFixture);

BOOST_AUTO_TEST_CASE(ReturnsEqualRenderStatesForSameConfiguration) {
	auto configuration = graphics::RenderState::Configuration();
	configuration.frontCounterClockwise = true;
	configuration.blendingEnabled = false;
	configuration.cullMode = graphics::RenderState::CullMode::FRONT;
	configuration.fillMode = graphics::RenderState::FillMode::WIREFRAME;

	auto renderStateL = RenderState(graphicsDevice(), configuration);
	auto renderStateR = RenderState(graphicsDevice(), configuration);

	BOOST_CHECK(renderStateL == renderStateR);
}

BOOST_AUTO_TEST_CASE(ReturnsUnequalRenderStatesForDifferentConfiguration) {
	auto configuration = graphics::RenderState::Configuration();
	configuration.frontCounterClockwise = true;
	configuration.blendingEnabled = false;
	configuration.cullMode = graphics::RenderState::CullMode::FRONT;
	configuration.fillMode = graphics::RenderState::FillMode::WIREFRAME;

	auto renderStateL = RenderState(graphicsDevice(), configuration);

	configuration.blendingEnabled = !configuration.blendingEnabled;

	auto renderStateR = RenderState(graphicsDevice(), configuration);

	BOOST_CHECK(renderStateL != renderStateR);
}

BOOST_AUTO_TEST_SUITE_END(/* RenderStateFactoryTestSuite */);

} // anonymous namespace
