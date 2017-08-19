#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <sstream>
#include <iostream>
#include <functional>

#include "dormouse-engine/logger/appender/ConsoleAppender.hpp"
#include "dormouse-engine/logger/layout/EmptyLayout.hpp"
#include "dormouse-engine/essentials/raii-helper.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;
using namespace dormouse_engine::logger::appender;

namespace {

BOOST_AUTO_TEST_SUITE(LoggerTestSuite);
BOOST_AUTO_TEST_SUITE(AppenderTestSuite);
BOOST_AUTO_TEST_SUITE(ConsoleAppenderTestSuite);

BOOST_AUTO_TEST_CASE(WritesToClog) {
	const std::string testString("test string");

	std::ostringstream output;

	{
		auto clogRdbuf = std::clog.rdbuf(output.rdbuf());
		essentials::RaiiHelper clogReset(
			[&]() { std::clog.rdbuf(clogRdbuf); }
			);

		layout::LayoutSharedPtr layout(new layout::EmptyLayout);
		ConsoleAppender appender(Level::TRACE, layout);

		appender.append(Level::INFO, Context(), testString);

		BOOST_CHECK_EQUAL(output.str(), layout->format(Level::INFO, Context(), testString));
	}
}

BOOST_AUTO_TEST_SUITE_END(/* ConsoleAppenderTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* AppenderTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* LoggerTestSuite */);

} // anonymous namespace
