#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <boost/regex.hpp>

#include "dormouse-engine/logger/layout/DetailedLayout.hpp"

namespace /* anonymous */ {

using namespace dormouse_engine;
using namespace dormouse_engine::logger;
using namespace dormouse_engine::logger::layout;

bool regexMatch(const boost::regex& regex, const std::string& string) {
	return boost::regex_match(string, regex);
}

BOOST_AUTO_TEST_SUITE(LoggerTestSuite);
BOOST_AUTO_TEST_SUITE(LayoutTestSuite);
BOOST_AUTO_TEST_SUITE(DetailedLayoutTestSuite);

BOOST_AUTO_TEST_CASE(InvalidPrecisionThrows) {
	BOOST_CHECK_THROW(DetailedLayout layout(Clock::Precision::UNKNOWN), exceptions::LogicError);
}

BOOST_AUTO_TEST_CASE(DetailedLayoutWithSecsDecoratesLoggedText) {
	DetailedLayout layout(Clock::Precision::SECONDS);

	std::string timeRegex = "\\d\\d\\.\\d\\d\\.\\d\\d\\d\\d \\d\\d:\\d\\d:\\d\\d";

	BOOST_CHECK_PREDICATE(
		regexMatch,
		(boost::regex('^' + timeRegex + " @ file:0 \\[CRITICAL\\] - category-name\ntext\n\n$"))
		(layout.format(Level::CRITICAL, Context("category-name", "file", 0, "function"), "text"))
		);
}

BOOST_AUTO_TEST_CASE(DetailedLayoutWithMillisDecoratesLoggedText) {
	DetailedLayout layout(Clock::Precision::MILLISECONDS);

	std::string timeRegex = "\\d\\d\\.\\d\\d\\.\\d\\d\\d\\d \\d\\d:\\d\\d:\\d\\d\\.\\d\\d\\d";

	BOOST_CHECK_PREDICATE(
		regexMatch,
		(boost::regex('^' + timeRegex + " @ file:0 \\[CRITICAL\\] - category-name\ntext\n\n$"))
		(layout.format(Level::CRITICAL, Context("category-name", "file", 0, "function"), "text"))
		);
}

BOOST_AUTO_TEST_CASE(DetailedLayoutWithMicrosDecoratesLoggedText) {
	DetailedLayout layout(Clock::Precision::MICROSECONDS);

	std::string timeRegex = "\\d\\d\\.\\d\\d\\.\\d\\d\\d\\d \\d\\d:\\d\\d:\\d\\d\\.\\d\\d\\d\\d\\d\\d";

	BOOST_CHECK_PREDICATE(
		regexMatch,
		(boost::regex('^' + timeRegex + " @ file:0 \\[CRITICAL\\] - category-name\ntext\n\n$"))
		(layout.format(Level::CRITICAL, Context("category-name", "file", 0, "function"), "text"))
		);
}

BOOST_AUTO_TEST_SUITE_END(/* DetailedLayoutTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* LayoutTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* LoggerTestSuite */);

} // anonymous namespace
