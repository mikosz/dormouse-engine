#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/renderer/command/CommandKey.hpp"

using namespace dormouse_engine::renderer::command;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(CommandKeyTestSuite);

BOOST_AUTO_TEST_CASE(CommandKeyOrdering) {
	auto lower = CommandKey();
	auto midLower = CommandKey();
	auto midHigher = CommandKey();
	auto higher = CommandKey();

	lower.setMaterialId(1u);
	midLower.setMaterialId(2u);
	midHigher.setFullscreenLayerId(FullscreenLayerId::HUD);
	higher.setFullscreenLayerId(FullscreenLayerId::DEBUG);

	BOOST_CHECK(lower.hash() < midLower.hash());
	BOOST_CHECK(midLower.hash() < midHigher.hash());
	BOOST_CHECK(midHigher.hash() < higher.hash());

}

BOOST_AUTO_TEST_SUITE_END(/* CommandKeyTestSuite */);

} // anonymous namespace
