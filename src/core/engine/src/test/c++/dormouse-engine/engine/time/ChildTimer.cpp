#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <thread>

#include "dormouse-engine/engine/time/ChildTimer.hpp"
#include "dormouse-engine/engine/time/WallClock.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::engine::time;
using namespace std::chrono_literals;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(EngineTimerChildTimerTestSuite);

BOOST_AUTO_TEST_CASE(DefaultChildTimerPassesTimeFromParent) {
	auto clock = WallClock();
	auto childTimer = ChildTimer(essentials::make_observer(&clock));

	clock.tick();
	std::this_thread::sleep_for(1ms);
	clock.tick();

	BOOST_CHECK_EQUAL(childTimer.lastFrameDuration(), clock.lastFrameDuration());
}

BOOST_AUTO_TEST_CASE(ChildTimerCanModifyParentTimes) {
	auto clock = WallClock();
	auto childTimer = ChildTimer(essentials::make_observer(&clock));

	childTimer.multiplier() = 2.0f;

	clock.tick();
	std::this_thread::sleep_for(1ms);
	clock.tick();

	BOOST_CHECK_EQUAL(childTimer.lastFrameDuration(), 2.0f * clock.lastFrameDuration());
}

BOOST_AUTO_TEST_CASE(TimersCanCreateAHierarchy) {
	auto clock = WallClock();

	auto commonParent = ChildTimer(essentials::make_observer(&clock));
	auto independentChild = ChildTimer(essentials::make_observer(&commonParent));
	auto subParent = ChildTimer(essentials::make_observer(&commonParent));
	auto dependentChild = ChildTimer(essentials::make_observer(&subParent));

	commonParent.multiplier() = 2.0f;
	independentChild.multiplier() = 3.0f;
	subParent.multiplier() = -1.0f;
	dependentChild.multiplier() = 4.0f;

	clock.tick();
	std::this_thread::sleep_for(1ms);
	clock.tick();

	BOOST_CHECK_EQUAL(commonParent.lastFrameDuration(), 2.0f * clock.lastFrameDuration());
	BOOST_CHECK_EQUAL(independentChild.lastFrameDuration(), 6.0f * clock.lastFrameDuration());
	BOOST_CHECK_EQUAL(subParent.lastFrameDuration(), -2.0f * clock.lastFrameDuration());
	BOOST_CHECK_EQUAL(dependentChild.lastFrameDuration(), -8.0f * clock.lastFrameDuration());
}

BOOST_AUTO_TEST_SUITE_END(/* EngineTimerChildTimerTestSuite */);

} // anonymous namespace
