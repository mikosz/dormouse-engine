#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <thread>

#include "dormouse-engine/engine/timer/Clock.hpp"
#include "dormouse-engine/engine/timer/ChildTimer.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::engine::timer;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(EngineTimerChildTimerTestSuite);

BOOST_AUTO_TEST_CASE(DefaultChildTimerPassesTimeFromParent) {
	
}

BOOST_AUTO_TEST_SUITE_END(/* EngineTimerChildTimerTestSuite */);

} // anonymous namespace
