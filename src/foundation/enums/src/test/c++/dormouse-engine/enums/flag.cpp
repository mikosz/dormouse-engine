#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/enums/flag.hpp"

namespace /* anonymous */ {


DE_FLAG(TestFlag,
	(VALUE0)
	(VALUE1)
	(VALUE2)
	);


DE_FLAG_VALUES(TestFlagValues,
	(VALUE3)(3)
	(VALUE4)(4)
	);

class C {
public:

	DE_MEMBER_FLAG(MemberTestFlag,
		(VALUE0)
		(VALUE1)
		);

	DE_MEMBER_FLAG_VALUES(MemberTestFlagValues,
		(VALUE3)(3)
		(VALUE4)(4)
		);
};

BOOST_AUTO_TEST_SUITE(FlagTestSuite);

BOOST_AUTO_TEST_CASE(DefaultFlagValuesAreBits) {
	BOOST_CHECK_EQUAL(static_cast<int>(TestFlag::VALUE0), 1 << 0);
	BOOST_CHECK_EQUAL(static_cast<int>(TestFlag::VALUE1), 1 << 1);
	BOOST_CHECK_EQUAL(static_cast<int>(TestFlag::VALUE2), 1 << 2);
}

BOOST_AUTO_TEST_CASE(ProvidedFlagValuesAreSet) {
	BOOST_CHECK_EQUAL(static_cast<int>(TestFlagValues::VALUE3), 3);
	BOOST_CHECK_EQUAL(static_cast<int>(TestFlagValues::VALUE4), 4);
}

BOOST_AUTO_TEST_CASE(FlagToStringYieldsName) {
	BOOST_CHECK_EQUAL(toString(TestFlag::VALUE0), "VALUE0");
	BOOST_CHECK_EQUAL(toString(TestFlag::VALUE1), "VALUE1");
	BOOST_CHECK_EQUAL(toString(TestFlag::VALUE2), "VALUE2");
	BOOST_CHECK_EQUAL(toString(TestFlagValues::VALUE3), "VALUE3");
	BOOST_CHECK_EQUAL(toString(TestFlagValues::VALUE4), "VALUE4");
}

BOOST_AUTO_TEST_CASE(FlagFromStringReadsDefaultValueName) {
	TestFlag value;
	fromString(value, "VALUE0");
	BOOST_CHECK_EQUAL(value, TestFlag::VALUE0);
	fromString(value, "VALUE1");
	BOOST_CHECK_EQUAL(value, TestFlag::VALUE1);
	fromString(value, "VALUE2");
	BOOST_CHECK_EQUAL(value, TestFlag::VALUE2);
}

BOOST_AUTO_TEST_CASE(FlagFromStringReadsSetValueName) {
	TestFlagValues value;
	fromString(value, "VALUE3");
	BOOST_CHECK_EQUAL(value, TestFlagValues::VALUE3);
	fromString(value, "VALUE4");
	BOOST_CHECK_EQUAL(value, TestFlagValues::VALUE4);
}

BOOST_AUTO_TEST_CASE(FlagFromStringReadsMemberSetValueName) {
	C::MemberTestFlagValues value;
	fromString(value, "VALUE3");
	BOOST_CHECK_EQUAL(value, C::MemberTestFlagValues::VALUE3);
	fromString(value, "VALUE4");
	BOOST_CHECK_EQUAL(value, C::MemberTestFlagValues::VALUE4);
}

BOOST_AUTO_TEST_CASE(FlagFromStringThrowsOnBadName) {
	TestFlag value;
	BOOST_CHECK_THROW(fromString(value, "NOT_A_VALUE_STRING"), std::out_of_range); // TODO: use a custom exception
}

BOOST_AUTO_TEST_CASE(AllFlagValuesContainsAllValues) {
	const auto& allValues = allTestFlagValues();

	BOOST_REQUIRE_EQUAL(allValues.size(), 3);
	BOOST_CHECK_EQUAL(allValues.count(TestFlag::VALUE0), 1);
	BOOST_CHECK_EQUAL(allValues.count(TestFlag::VALUE1), 1);
	BOOST_CHECK_EQUAL(allValues.count(TestFlag::VALUE2), 1);
}

BOOST_AUTO_TEST_CASE(AllowsMemberFlagAsMember) {
	BOOST_CHECK_EQUAL(static_cast<int>(C::MemberTestFlag::VALUE0), 1 << 0);
	BOOST_CHECK_EQUAL(static_cast<int>(C::MemberTestFlag::VALUE1), 1 << 1);

	BOOST_CHECK_EQUAL(static_cast<int>(C::MemberTestFlagValues::VALUE3), 3);
	BOOST_CHECK_EQUAL(static_cast<int>(C::MemberTestFlagValues::VALUE4), 4);
}

BOOST_AUTO_TEST_CASE(FromIntegralReturnsDefaultValues) {
	TestFlag value;
	fromIntegral(value, 1 << 0);
	BOOST_CHECK_EQUAL(value, TestFlag::VALUE0);
	fromIntegral(value, 1 << 1);
	BOOST_CHECK_EQUAL(value, TestFlag::VALUE1);
	fromIntegral(value, 1 << 2);
	BOOST_CHECK_EQUAL(value, TestFlag::VALUE2);
}

BOOST_AUTO_TEST_CASE(FromIntegralReturnsSetValues) {
	TestFlagValues value;
	fromIntegral(value, 3);
	BOOST_CHECK_EQUAL(value, TestFlagValues::VALUE3);
	fromIntegral(value, 4);
	BOOST_CHECK_EQUAL(value, TestFlagValues::VALUE4);
}

BOOST_AUTO_TEST_CASE(FromIntegralReturnsMemberDefaultValues) {
	C::MemberTestFlag value;
	fromIntegral(value, 1 << 0);
	BOOST_CHECK_EQUAL(value, C::MemberTestFlag::VALUE0);
	fromIntegral(value, 1 << 1);
	BOOST_CHECK_EQUAL(value, C::MemberTestFlag::VALUE1);
}

BOOST_AUTO_TEST_CASE(FromIntegralReturnsMemberSetValues) {
	C::MemberTestFlagValues value;
	fromIntegral(value, 3);
	BOOST_CHECK_EQUAL(value, C::MemberTestFlagValues::VALUE3);
	fromIntegral(value, 4);
	BOOST_CHECK_EQUAL(value, C::MemberTestFlagValues::VALUE4);
}

BOOST_AUTO_TEST_CASE(FromIntegralThrowsOnInvalidValues) {
	TestFlag value;
	BOOST_CHECK_THROW(fromIntegral(value, -1), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END(/* FlagTestSuite */);

} // anonymous namespace
