#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <sstream>
#include <iostream>
#include <functional>

#include <boost/mpl/list.hpp>

#include "dormouse-engine/logger/Logger.hpp"
#include "dormouse-engine/logger/macros.hpp"
#include "dormouse-engine/logger/appender/ConsoleAppender.hpp"
#include "dormouse-engine/logger/layout/EmptyLayout.hpp"

#include "dormouse-engine/essentials/raii-helper.hpp"

namespace {

using namespace dormouse_engine;
using namespace dormouse_engine::logger;

BOOST_AUTO_TEST_SUITE(LoggerTestSuite);

BOOST_AUTO_TEST_CASE(LogsOnRequiredLevelsTest) {
	std::ostringstream output;

	{
		auto clogRdbuf = std::clog.rdbuf(output.rdbuf());
		essentials::RaiiHelper clogReset([&]() { std::clog.rdbuf(clogRdbuf); });

		Logger logger(Level::INFO);

		layout::LayoutSharedPtr layout(new layout::EmptyLayout);
		logger.addAppender(appender::AppenderSharedPtr(new appender::ConsoleAppender(Level::TRACE, layout)));

		logger.log(Level::CRITICAL) << "critical";
		logger.log(Level::ERROR) << "error";
		logger.log(Level::WARNING) << "warning";
		logger.log(Level::INFO) << "info";
		logger.log(Level::DEBUG) << "debug";
		logger.log(Level::TRACE) << "trace";

		logger.critical() << "critical";
		logger.error() << "error";
		logger.warning() << "warning";
		logger.info() << "info";
		logger.debug() << "debug";
		logger.trace() << "trace";
	}

	const std::string EXPECTED =
		"critical\n"
		"error\n"
		"warning\n"
		"info\n"
		;

	BOOST_CHECK_EQUAL(output.str(), EXPECTED + EXPECTED);
}

BOOST_AUTO_TEST_SUITE_END(/* LoggerTestSuite */);

} // anonymous namespace
