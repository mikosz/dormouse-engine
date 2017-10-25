#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/renderer/control/RenderState.hpp"
#include "dormouse-engine/tester/App.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::control;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(RenderStateFactoryTestSuite);

BOOST_AUTO_TEST_CASE(ReturnsEqualRenderStatesForSameConfiguration) {
	auto& app = *tester::GlobalApp::instance();

	const auto configuration = RenderState::OPAQUE;

	auto renderStateL = RenderState(app.graphicsDevice(), configuration);
	auto renderStateR = RenderState(app.graphicsDevice(), configuration);

	BOOST_CHECK(renderStateL == renderStateR);
}

BOOST_AUTO_TEST_CASE(ReturnsUnequalRenderStatesForDifferentConfiguration) {
	auto& app = *tester::GlobalApp::instance();

	auto configuration = RenderState::OPAQUE;

	auto renderStateL = RenderState(app.graphicsDevice(), configuration);

	configuration.blendingEnabled = !configuration.blendingEnabled;

	auto renderStateR = RenderState(app.graphicsDevice(), configuration);

	BOOST_CHECK(renderStateL != renderStateR);
}

BOOST_AUTO_TEST_SUITE_END(/* RenderStateFactoryTestSuite */);

} // anonymous namespace
