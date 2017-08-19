#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <algorithm>
#include <vector>
#include <sstream>

#include "dormouse-engine/essentials/InfixOstreamIterator.hpp"

using namespace dormouse_engine;

using namespace std::string_literals;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(InfixOstreamIteratorTestSuite);

BOOST_AUTO_TEST_CASE(SingleElementPrintsSingleElement) {
	const auto vec = std::vector<int>{ 1 };

	auto oss = std::ostringstream();
	std::copy(vec.begin(), vec.end(), InfixOstreamIterator<int>(oss, " + "));

	BOOST_CHECK_EQUAL(oss.str(), "1"s);
}

BOOST_AUTO_TEST_CASE(MultipleElementsAreDelimited) {
	const auto vec = std::vector<int>{ 1, 2, 3 };

	auto oss = std::ostringstream();
	std::copy(vec.begin(), vec.end(), InfixOstreamIterator<int>(oss, " + "));

	BOOST_CHECK_EQUAL(oss.str(), "1 + 2 + 3"s);
}

BOOST_AUTO_TEST_CASE(NoDelimiterPrintsConjoined) {
	const auto vec = std::vector<int>{ 1, 2, 3 };

	auto oss = std::ostringstream();
	std::copy(vec.begin(), vec.end(), InfixOstreamIterator<int>(oss));

	BOOST_CHECK_EQUAL(oss.str(), "123"s);
}

BOOST_AUTO_TEST_SUITE_END(/* InfixOstreamIteratorTestSuite */);

} // anonymous namespace
