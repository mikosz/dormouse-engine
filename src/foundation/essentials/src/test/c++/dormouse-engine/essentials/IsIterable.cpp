#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <vector>

#include "dormouse-engine/essentials/IsIterable.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::essentials;

namespace /* anonymous */ {

struct CustomIterable {
};

int* begin(CustomIterable) {
	return nullptr;
}

int* end(CustomIterable) {
	return nullptr;
}

BOOST_AUTO_TEST_SUITE(IsIterableTestSuite);

BOOST_AUTO_TEST_CASE(VectorIntIsIterable) {
	BOOST_CHECK(static_cast<bool>(IsIterable<std::vector<int> >::value));
}

BOOST_AUTO_TEST_CASE(DoubleArrayIsIterable) {
	BOOST_CHECK(static_cast<bool>(IsIterable<double[42]>::value));
}

BOOST_AUTO_TEST_CASE(CustomIterableClassIsIterable) {
	BOOST_CHECK(static_cast<bool>(IsIterable<CustomIterable>::value));
}

BOOST_AUTO_TEST_CASE(IntIsNotIterable) {
	BOOST_CHECK(!static_cast<bool>(IsIterable<int>::value));
}

BOOST_AUTO_TEST_SUITE_END(/* IsIterableTestSuite */);

} // anonymous namespace
