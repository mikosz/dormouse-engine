#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <thread>
#include <chrono>

#include "dormouse-engine/engine/time/WallClock.hpp"

using namespace dormouse_engine::engine::time;
using namespace std::chrono_literals;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(EngineTimerClockTestSuite);

BOOST_AUTO_TEST_CASE(MeasuresTime) {
	auto clock = WallClock();

	BOOST_CHECK_EQUAL(clock.lastFrameDuration(), 0.0f);

	auto verifyingClock = std::chrono::high_resolution_clock();

	auto start = verifyingClock.now();
	clock.tick();
	auto preSleep = verifyingClock.now();
	std::this_thread::sleep_for(10ms);
	auto postSleep = verifyingClock.now();
	clock.tick();
	auto end = verifyingClock.now();

	const auto lastFrameDuration = clock.lastFrameDuration();

	BOOST_CHECK_EQUAL(lastFrameDuration, clock.lastFrameDuration());
	
	BOOST_CHECK_GE(lastFrameDuration, 0.010f);
	BOOST_CHECK_LE(lastFrameDuration, 0.011f);
	BOOST_CHECK(std::chrono::microseconds(static_cast<int>(lastFrameDuration * 1'000'000.0f)) >= (postSleep - preSleep));
	BOOST_CHECK(std::chrono::microseconds(static_cast<int>(lastFrameDuration * 1'000'000.0f)) <= (end - start));
}

BOOST_AUTO_TEST_CASE(CanApplyLowerFrameTimeLimit) {
	auto verifyingClock = std::chrono::high_resolution_clock();

	const auto minDuration = 1.0f / 60.0f;

	auto clock = WallClock(minDuration, WallClock::NO_LIMIT);
	auto start = verifyingClock.now();
	clock.tick();
	auto end = verifyingClock.now();

	BOOST_CHECK_GE(clock.lastFrameDuration(), minDuration);
	BOOST_CHECK(end - start >= std::chrono::milliseconds(static_cast<int>(minDuration * 1'000.0f)));
}

BOOST_AUTO_TEST_CASE(CanApplyUpperFrameTimeLimit) {
	auto verifyingClock = std::chrono::high_resolution_clock();

	const auto maxDuration = 1.0f / 60.0f;
	const auto sleepDuration = std::chrono::milliseconds(static_cast<int>(maxDuration * 1'000.0f) + 10);

	auto clock = WallClock(WallClock::NO_LIMIT, maxDuration);
	auto start = verifyingClock.now();
	std::this_thread::sleep_for(sleepDuration);
	clock.tick();
	auto end = verifyingClock.now();

	BOOST_CHECK_EQUAL(clock.lastFrameDuration(), maxDuration);
	BOOST_CHECK(end - start >= sleepDuration);
}

BOOST_AUTO_TEST_SUITE_END(/* EngineTimerClockTestSuite */);

} // anonymous namespace
