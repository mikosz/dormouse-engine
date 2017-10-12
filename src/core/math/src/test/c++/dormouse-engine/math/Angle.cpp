#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/math/Angle.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine;
using namespace dormouse_engine::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(PulpTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathAngleTestSuite);

BOOST_AUTO_TEST_CASE(AngleIsConstructibleWithRadians) {
	constexpr auto a = 3.0_rad;
	static_assert(a.radians() == 3.0f);
}

BOOST_AUTO_TEST_CASE(AngleIsConstructibleWithDegrees) {
	constexpr auto a = 90.0_deg;
	static_assert(a.degrees() == 90.0f);
}

BOOST_AUTO_TEST_CASE(AngleConvertsRadiansToDegrees) {
	constexpr auto a = radians(PI);
	static_assert(a.degrees() == 180.0f);
}

BOOST_AUTO_TEST_CASE(AngleConvertsDegreesToRadians) {
	constexpr auto a = 360.0_deg;
	static_assert(a.radians() == a.full().radians());
}

BOOST_AUTO_TEST_CASE(AnglesAreAdditive) {
	constexpr auto lhs = Angle::right();
	constexpr auto rhs = Angle::halfFull();

	BOOST_CHECK_EQUAL(lhs + rhs, radians(1.5f * PI)); // TODO: constexpr addition et al?
	BOOST_CHECK_EQUAL(rhs - lhs, Angle::right());
}

BOOST_AUTO_TEST_CASE(AnglesAreMultiplicativeByScalar) {
	BOOST_CHECK_EQUAL(Angle::right() * 4.0f, Angle::full());
	BOOST_CHECK_EQUAL(Angle::halfFull() / 2.0f, Angle::right());
}

BOOST_AUTO_TEST_CASE(AnglesAreComparable) {
	BOOST_CHECK_EQUAL(degrees(180.0f), radians(PI));
	BOOST_CHECK_NE(degrees(180.0f), degrees(180.001f));
	BOOST_CHECK_LE(degrees(180.0f), radians(PI));
	BOOST_CHECK_LT(degrees(179.0f), radians(PI));
	BOOST_CHECK_GT(radians(4.0f), degrees(180.0f));
	BOOST_CHECK_GE(radians(4.0f), degrees(180.0f));
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathAngleTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // anonymous namespace
