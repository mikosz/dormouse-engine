#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/enums/enum.hpp"

namespace /* anonymous */ {

DE_ENUM(TestEnum,
	(VALUE0)
	(VALUE1)
	(VALUE2)
	);

DE_ENUM_VALUES(TestEnumValues,
	(TRIANGLE)(3)
	(SQUARE)(4)
	);

class C {
public:

	DE_MEMBER_ENUM(MemberTestEnum,
		(VALUE0)
		(VALUE1)
		);

	DE_MEMBER_ENUM_VALUES(MemberTestEnumValues,
		(TRIANGLE)(3)
		(SQUARE)(4)
		);
};

BOOST_AUTO_TEST_SUITE(EnumTestSuite);

BOOST_AUTO_TEST_CASE(DefaultEnumValuesAreIncrementingIntegers) {
	BOOST_CHECK_EQUAL(static_cast<int>(TestEnum::VALUE0), 0);
	BOOST_CHECK_EQUAL(static_cast<int>(TestEnum::VALUE1), 1);
	BOOST_CHECK_EQUAL(static_cast<int>(TestEnum::VALUE2), 2);
}

BOOST_AUTO_TEST_CASE(ProvidedEnumValuesAreSet) {
	BOOST_CHECK_EQUAL(static_cast<int>(TestEnumValues::TRIANGLE), 3);
	BOOST_CHECK_EQUAL(static_cast<int>(TestEnumValues::SQUARE), 4);
}

BOOST_AUTO_TEST_CASE(EnumToStringYieldsName) {
	BOOST_CHECK_EQUAL(toString(TestEnum::VALUE0), "VALUE0");
	BOOST_CHECK_EQUAL(toString(TestEnum::VALUE1), "VALUE1");
	BOOST_CHECK_EQUAL(toString(TestEnum::VALUE2), "VALUE2");
	BOOST_CHECK_EQUAL(toString(TestEnumValues::TRIANGLE), "TRIANGLE");
	BOOST_CHECK_EQUAL(toString(TestEnumValues::SQUARE), "SQUARE");
}

BOOST_AUTO_TEST_CASE(EnumFromStringReadsDefaultValueName) {
	TestEnum value;
	fromString(value, "VALUE0");
	BOOST_CHECK_EQUAL(value, TestEnum::VALUE0);
	fromString(value, "VALUE1");
	BOOST_CHECK_EQUAL(value, TestEnum::VALUE1);
	fromString(value, "VALUE2");
	BOOST_CHECK_EQUAL(value, TestEnum::VALUE2);
}

BOOST_AUTO_TEST_CASE(EnumFromStringReadsSetValueName) {
	TestEnumValues value;
	fromString(value, "TRIANGLE");
	BOOST_CHECK_EQUAL(value, TestEnumValues::TRIANGLE);
	fromString(value, "SQUARE");
	BOOST_CHECK_EQUAL(value, TestEnumValues::SQUARE);
}

BOOST_AUTO_TEST_CASE(EnumFromStringReadsMemberSetValueName) {
	C::MemberTestEnumValues value;
	fromString(value, "TRIANGLE");
	BOOST_CHECK_EQUAL(value, C::MemberTestEnumValues::TRIANGLE);
	fromString(value, "SQUARE");
	BOOST_CHECK_EQUAL(value, C::MemberTestEnumValues::SQUARE);
}

BOOST_AUTO_TEST_CASE(EnumFromStringThrowsOnBadName) {
	TestEnum value;
	BOOST_CHECK_THROW(fromString(value, "NOT_A_VALUE_STRING"), std::out_of_range); // TODO: use a custom exception
}

BOOST_AUTO_TEST_CASE(AllEnumValuesContainsAllValues) {
	const auto& allValues = allTestEnumValues();

	BOOST_REQUIRE_EQUAL(allValues.size(), 3);
	BOOST_CHECK_EQUAL(allValues.count(TestEnum::VALUE0), 1);
	BOOST_CHECK_EQUAL(allValues.count(TestEnum::VALUE1), 1);
	BOOST_CHECK_EQUAL(allValues.count(TestEnum::VALUE2), 1);
}

BOOST_AUTO_TEST_CASE(AllowsMemberEnumAsMember) {
	BOOST_CHECK_EQUAL(static_cast<int>(C::MemberTestEnum::VALUE0), 0);
	BOOST_CHECK_EQUAL(static_cast<int>(C::MemberTestEnum::VALUE1), 1);

	BOOST_CHECK_EQUAL(static_cast<int>(C::MemberTestEnumValues::TRIANGLE), 3);
	BOOST_CHECK_EQUAL(static_cast<int>(C::MemberTestEnumValues::SQUARE), 4);
}

BOOST_AUTO_TEST_CASE(FromIntegralReturnsDefaultValues) {
	TestEnum value;
	fromIntegral(value, 0);
	BOOST_CHECK_EQUAL(value, TestEnum::VALUE0);
	fromIntegral(value, 1);
	BOOST_CHECK_EQUAL(value, TestEnum::VALUE1);
	fromIntegral(value, 2);
	BOOST_CHECK_EQUAL(value, TestEnum::VALUE2);
}

BOOST_AUTO_TEST_CASE(FromIntegralReturnsSetValues) {
	TestEnumValues value;
	fromIntegral(value, 3);
	BOOST_CHECK_EQUAL(value, TestEnumValues::TRIANGLE);
	fromIntegral(value, 4);
	BOOST_CHECK_EQUAL(value, TestEnumValues::SQUARE);
}

BOOST_AUTO_TEST_CASE(FromIntegralReturnsMemberDefaultValues) {
	C::MemberTestEnum value;
	fromIntegral(value, 0);
	BOOST_CHECK_EQUAL(value, C::MemberTestEnum::VALUE0);
	fromIntegral(value, 1);
	BOOST_CHECK_EQUAL(value, C::MemberTestEnum::VALUE1);
}

BOOST_AUTO_TEST_CASE(FromIntegralReturnsMemberSetValues) {
	C::MemberTestEnumValues value;
	fromIntegral(value, 3);
	BOOST_CHECK_EQUAL(value, C::MemberTestEnumValues::TRIANGLE);
	fromIntegral(value, 4);
	BOOST_CHECK_EQUAL(value, C::MemberTestEnumValues::SQUARE);
}

BOOST_AUTO_TEST_CASE(FromIntegralThrowsOnInvalidValues) {
	TestEnum value;
	BOOST_CHECK_THROW(fromIntegral(value, -1), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END(/* EnumTestSuite */);

} // anonymous namespace
