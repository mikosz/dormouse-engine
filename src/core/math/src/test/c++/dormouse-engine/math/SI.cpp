#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <string>
#include <type_traits>

#include "dormouse-engine/math/SI.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::math;
using namespace std::string_literals;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(MathSITestSuite);

BOOST_AUTO_TEST_CASE(ValueSizeAndAlignmentIsSameAsFloat) {
	static_assert(sizeof(Mass) == sizeof(float));
	static_assert(alignof(Mass) == alignof(float));
}

BOOST_AUTO_TEST_CASE(ValuesAreMultiplicativeWithFloats) {
	constexpr auto lm = 2.0f * 1_kg;
	constexpr auto rm = 1_kg * 2.0f;

	BOOST_CHECK_EQUAL(lm, rm);
}

BOOST_AUTO_TEST_CASE(KilogramIs1000Grams) {
	constexpr auto g = 1_g;
	constexpr auto kg = 1_kg;

	constexpr auto kg_in_grams = kg.value<Gram>();
	BOOST_CHECK_CLOSE(kg_in_grams, 1000.0f, 0.0001f);

	constexpr auto g_in_kg = g.value<Kilogram>();
	BOOST_CHECK_CLOSE(g_in_kg, 0.001f, 0.0001f);
}

BOOST_AUTO_TEST_CASE(SpeedIsLengthDividedByTime) {
	constexpr auto s = 1.0_m / 2.0_s;
	static_assert(std::is_same_v<decltype(s), const Speed>);
	BOOST_CHECK_CLOSE(s.value<MetresPerSecond>(), 0.5, 0.0001f);
}

BOOST_AUTO_TEST_CASE(KPHisConvertibleToMPS) {
	constexpr auto mps = 60_m / 2_s;
	constexpr auto kph = 54_km / 0.5_h;

	BOOST_CHECK_CLOSE(mps.value<MetresPerSecond>(), (30_mps).value<MetresPerSecond>(), 0.0001f);
	BOOST_CHECK_CLOSE(kph.value<KilometresPerHour>(), (108_kph).value<KilometresPerHour>(), 0.0001f);

	BOOST_CHECK_CLOSE(mps.value<KilometresPerHour>(), kph.value<KilometresPerHour>(), 0.0001f);
	BOOST_CHECK_CLOSE(kph.value<MetresPerSecond>(), mps.value<MetresPerSecond>(), 0.0001f);
}

BOOST_AUTO_TEST_CASE(NewtonIsKgTimesMetreBySecondSquared) {
	constexpr auto f = 1.0_N;
	static_assert(std::is_same_v<decltype(f), const Force>);
	static_assert(f == (1.0_kg * 1.0_mps) / 1.0_s);
}

BOOST_AUTO_TEST_CASE(ValuesPrintInInitialisedUnits) {
	using std::to_string;

	// base
	BOOST_CHECK_EQUAL(to_string(42.5_m), "42.5 [m]"s);
	BOOST_CHECK_EQUAL(to_string(42_kg), "42 [kg]"s);
	BOOST_CHECK_EQUAL(to_string(42_s), "42 [s]"s);

	BOOST_CHECK_EQUAL(to_string(42_m), "42 [m]"s);

	// complex
	BOOST_CHECK_EQUAL(to_string(2_mps), "2 [m/s]"s);
	BOOST_CHECK_EQUAL(to_string(4_m / 2_s), "2 [m/s]"s);
	BOOST_CHECK_EQUAL(to_string(42_N), "42 [N]"s);
	BOOST_CHECK_EQUAL(to_string(1_kg * 1_m / (1_s * 1_s)), "1 [N]"s);

	// multiples
	BOOST_CHECK_EQUAL(to_string(3_km / 2_h), "1.5 [km/h]"s);
	BOOST_CHECK_EQUAL(to_string(1_g * 1_mps / 1_s), "1 [1/1000*kg*m/s^2]");
	BOOST_CHECK_EQUAL(to_string(Force(1_g * 1_mps / 1_s)), "0.001 [N]");
}

BOOST_AUTO_TEST_SUITE_END(/* MathSITestSuite */);

} // anonymous namespace
