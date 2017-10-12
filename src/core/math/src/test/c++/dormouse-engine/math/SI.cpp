#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <type_traits>

#include "dormouse-engine/math/SI.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(MathSITestSuite);

BOOST_AUTO_TEST_CASE(SpeedIsLengthDividedByTime) {
	constexpr auto s = 1.0_m / 2.0_s;
	static_assert(std::is_same_v<decltype(s), const Speed>);
	static_assert(s == 0.5_mps);
}

BOOST_AUTO_TEST_CASE(NewtonIsKgTimesMetreBySecondSquared) {
	constexpr auto f = 1.0_N;
	static_assert(std::is_same_v<decltype(f), const Force>);
	static_assert(f == (1.0_kg * 1.0_mps) / 1.0_s);
}

BOOST_AUTO_TEST_SUITE_END(/* MathSITestSuite */);

} // anonymous namespace
