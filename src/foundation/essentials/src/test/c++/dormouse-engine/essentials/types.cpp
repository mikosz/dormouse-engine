#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/essentials/types.hpp"

#include <type_traits>

namespace /* anonymous */ {

using namespace dormouse_engine::essentials;
using namespace std::string_literals;

BOOST_AUTO_TEST_SUITE(TypesTestSuite);

BOOST_AUTO_TEST_CASE(MakeArrayCreatesArrayOfInts) {
	constexpr auto a = makeArray(1, 2, 3, 4);
	static_assert(std::is_same_v<decltype(a)::value_type, int>);
	static_assert(a.size() == 4u);
}

BOOST_AUTO_TEST_CASE(MakeArrayCreatesArrayOfCommonType) {
	constexpr auto a = makeArray(1.0, 2.0, 3.0f, 4.0f);
	static_assert(std::is_same_v<decltype(a)::value_type, double>);
	static_assert(a.size() == 4u);
}

BOOST_AUTO_TEST_CASE(IsAnyPointerIsTrueForAllPointers) {
	static_assert(IsAnyPointer_v<int*>);
	static_assert(IsAnyPointer_v<const int*>);
	static_assert(IsAnyPointer_v<const volatile int* const>);

	static_assert(IsAnyPointer_v<std::unique_ptr<int>>);
	static_assert(IsAnyPointer_v<const std::unique_ptr<int>>);
	static_assert(IsAnyPointer_v<volatile std::shared_ptr<int>>);

	static_assert(IsAnyPointer_v<const observer_ptr<const int>>);
}

BOOST_AUTO_TEST_SUITE_END(/* TypesTestSuite */);

} // anonymous namespace
