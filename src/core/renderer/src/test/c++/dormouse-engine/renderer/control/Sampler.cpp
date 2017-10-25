#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/renderer/control/Sampler.hpp"
#include "dormouse-engine/tester/App.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::control;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(RendererSamplerTestSuite);

BOOST_AUTO_TEST_CASE(ReturnsEqualSamplersForSameConfiguration) {
	auto& app = *tester::GlobalApp::instance();

	const auto configuration = Sampler::WRAPPED_LINEAR;

	auto samplerL = Sampler(app.graphicsDevice(), configuration);
	auto samplerR = Sampler(app.graphicsDevice(), configuration);

	BOOST_CHECK(samplerL == samplerR);
}

BOOST_AUTO_TEST_CASE(ReturnsUnequalSamplersForDifferentConfiguration) {
	auto& app = *tester::GlobalApp::instance();

	auto configuration = Sampler::WRAPPED_LINEAR;

	auto samplerL = Sampler(app.graphicsDevice(), configuration);

	configuration.addressModeU = graphics::Sampler::AddressMode::CLAMP;

	auto samplerR = Sampler(app.graphicsDevice(), configuration);

	BOOST_CHECK(samplerL != samplerR);
}

BOOST_AUTO_TEST_SUITE_END(/* RendererSamplerTestSuite */);

} // anonymous namespace
