#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/enums/Mask.hpp"

namespace /* anonymous */ {

using namespace dormouse_engine;
using namespace dormouse_engine::enums;

DE_FLAG(TestFlag,
	(VALUE0)
	(VALUE1)
	(VALUE2)
	);

DE_FLAG_VALUES(OperatorTestFlag,
	(F01)(0b01)
	(F10)(0b10)
	(F11)(0b11)
	);

BOOST_AUTO_TEST_SUITE(MaskTestSuite);

BOOST_AUTO_TEST_CASE(FlagBitwiseOrYieldsMask) {
	const auto ff = TestFlag::VALUE0 | TestFlag::VALUE1;
	const auto mf = Mask<TestFlag>(TestFlag::VALUE0) | TestFlag::VALUE1;
	const auto fm = TestFlag::VALUE0 | Mask<TestFlag>(TestFlag::VALUE1);
	const auto mm = Mask<TestFlag>(TestFlag::VALUE0) | Mask<TestFlag>(TestFlag::VALUE1);

	BOOST_CHECK((std::is_same_v<decltype(ff), const Mask<TestFlag>>));
	BOOST_CHECK((std::is_same_v<decltype(mf), const Mask<TestFlag>>));
	BOOST_CHECK((std::is_same_v<decltype(fm), const Mask<TestFlag>>));
	BOOST_CHECK((std::is_same_v<decltype(mm), const Mask<TestFlag>>));
}

BOOST_AUTO_TEST_CASE(FlagBitwiseAndYieldsMask) {
	const auto ff = TestFlag::VALUE0 & TestFlag::VALUE1;
	const auto mf = Mask<TestFlag>(TestFlag::VALUE0) & TestFlag::VALUE1;
	const auto fm = TestFlag::VALUE0 & Mask<TestFlag>(TestFlag::VALUE1);
	const auto mm = Mask<TestFlag>(TestFlag::VALUE0) & Mask<TestFlag>(TestFlag::VALUE1);

	BOOST_CHECK((std::is_same_v<decltype(ff), const Mask<TestFlag>>));
	BOOST_CHECK((std::is_same_v<decltype(mf), const Mask<TestFlag>>));
	BOOST_CHECK((std::is_same_v<decltype(fm), const Mask<TestFlag>>));
	BOOST_CHECK((std::is_same_v<decltype(mm), const Mask<TestFlag>>));
}

BOOST_AUTO_TEST_CASE(FlagBitwiseXorYieldsMask) {
	const auto ff = TestFlag::VALUE0 ^ TestFlag::VALUE1;
	const auto mf = Mask<TestFlag>(TestFlag::VALUE0) ^ TestFlag::VALUE1;
	const auto fm = TestFlag::VALUE0 ^ Mask<TestFlag>(TestFlag::VALUE1);
	const auto mm = Mask<TestFlag>(TestFlag::VALUE0) ^ Mask<TestFlag>(TestFlag::VALUE1);

	BOOST_CHECK((std::is_same_v<decltype(ff), const Mask<TestFlag>>));
	BOOST_CHECK((std::is_same_v<decltype(mf), const Mask<TestFlag>>));
	BOOST_CHECK((std::is_same_v<decltype(fm), const Mask<TestFlag>>));
	BOOST_CHECK((std::is_same_v<decltype(mm), const Mask<TestFlag>>));
}

BOOST_AUTO_TEST_CASE(MaskIsCastableToBoolean) {
	BOOST_CHECK(Mask<TestFlag>(TestFlag::VALUE0));
	BOOST_CHECK(!Mask<TestFlag>());
}

BOOST_AUTO_TEST_CASE(MasksAreEqualityComparable) {
	const auto m0 = Mask<TestFlag>(TestFlag::VALUE0);
	const auto m00 = Mask<TestFlag>(TestFlag::VALUE0);
	const auto m1 = Mask<TestFlag>(TestFlag::VALUE1);

	BOOST_CHECK_EQUAL(m0, m00);
	BOOST_CHECK_NE(m0, m1);
}

BOOST_AUTO_TEST_CASE(MasksAreOrable) {
	BOOST_CHECK_EQUAL(OperatorTestFlag::F01 | OperatorTestFlag::F10, OperatorTestFlag::F11);
}

BOOST_AUTO_TEST_CASE(MasksAreAndable) {
	BOOST_CHECK_EQUAL(OperatorTestFlag::F01 & OperatorTestFlag::F11, OperatorTestFlag::F01);
}

BOOST_AUTO_TEST_CASE(MasksAreXorable) {
	BOOST_CHECK_EQUAL(OperatorTestFlag::F01 ^ OperatorTestFlag::F11, OperatorTestFlag::F10);
}

BOOST_AUTO_TEST_CASE(MasksAreNegatable) {
	BOOST_CHECK_EQUAL(
		~Mask<OperatorTestFlag>(OperatorTestFlag::F01) & OperatorTestFlag::F10,
		OperatorTestFlag::F10
		);
}

BOOST_AUTO_TEST_SUITE_END(/* MaskTestSuite */);

} // anonymous namespace
