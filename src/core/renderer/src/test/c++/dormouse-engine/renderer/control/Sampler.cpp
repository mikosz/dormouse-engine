#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/renderer/control/Sampler.hpp"
#include "dormouse-engine/tester/EngineTesterFixture.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::control;

namespace /* anonymous */ {

BOOST_FIXTURE_TEST_SUITE(RendererSamplerTestSuite, tester::RenderingFixture);

BOOST_AUTO_TEST_CASE(ReturnsEqualSamplersForSameConfiguration) {
	const auto configuration = Sampler::WRAPPED_LINEAR;

	auto samplerL = Sampler(graphicsDevice(), configuration);
	auto samplerR = Sampler(graphicsDevice(), configuration);

	BOOST_CHECK(samplerL == samplerR);
}

BOOST_AUTO_TEST_CASE(ReturnsUnequalSamplersForDifferentConfiguration) {
	auto configuration = Sampler::WRAPPED_LINEAR;

	auto samplerL = Sampler(graphicsDevice(), configuration);

	configuration.addressModeU = graphics::Sampler::AddressMode::CLAMP;

	auto samplerR = Sampler(graphicsDevice(), configuration);

	BOOST_CHECK(samplerL != samplerR);
}

BOOST_AUTO_TEST_SUITE_END(/* RendererSamplerTestSuite */);

} // anonymous namespace
