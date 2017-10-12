#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <thread>

#include "dormouse-engine/engine/timer/WallClock.hpp"

using namespace dormouse_engine::engine::timer;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(EngineTimerClockTestSuite);

BOOST_AUTO_TEST_CASE(MeasuresTime) {
	auto clock = WallClock();

	BOOST_CHECK_EQUAL(clock.lastFrameDuration(), 0.0f);

	clock.tick();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	clock.tick();

	const auto lastFrameDuration = clock.lastFrameDuration();

	BOOST_CHECK_EQUAL(lastFrameDuration, clock.lastFrameDuration());
	BOOST_CHECK_GE(lastFrameDuration, 0.010f);
	BOOST_CHECK_LE(lastFrameDuration, 0.011f);
}

BOOST_AUTO_TEST_SUITE_END(/* EngineTimerClockTestSuite */);

} // anonymous namespace
