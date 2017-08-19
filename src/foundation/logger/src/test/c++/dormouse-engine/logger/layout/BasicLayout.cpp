#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/logger/layout/BasicLayout.hpp"

namespace /* anonymous */ {

using namespace dormouse_engine;
using namespace dormouse_engine::logger;
using namespace dormouse_engine::logger::layout;

BOOST_AUTO_TEST_SUITE(LoggerTestSuite);
BOOST_AUTO_TEST_SUITE(LayoutTestSuite);
BOOST_AUTO_TEST_SUITE(BasicLayoutTestSuite);

BOOST_AUTO_TEST_CASE(BasicLayoutDecoratesLoggedText) {
	BasicLayout layout;

	BOOST_CHECK_EQUAL(
		layout.format(Level::CRITICAL, Context("category-name", "file", 0, "function"), "text"),
		"category-name [CRITICAL] - text\n"
		);
}

BOOST_AUTO_TEST_SUITE_END(/* BasicLayoutTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* LayoutTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* LoggerTestSuite */);

} // anonymous namespace
