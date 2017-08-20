#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <sstream>

#include "dormouse-engine/essentials/test-utils/test-utils.hpp"
#include "dormouse-engine/essentials/raii-helper.hpp"
#include "dormouse-engine/logger.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;

namespace {

BOOST_AUTO_TEST_SUITE(LoggerFunctionalTestSuite);

BOOST_AUTO_TEST_CASE(DefaultConfigurationPrintsInfoToStdlog) {
	std::ostringstream output;

	{
		auto clogBuf = std::clog.rdbuf(output.rdbuf());
		essentials::RaiiHelper clogReset(
			[&]() { std::clog.rdbuf(clogBuf); }
		);

		DE_LOGGER_LOCAL_CATEGORY("MACRO_LOGGING.DefaultConfigurationPrintsInfoToStdlog");

		DE_LOG_TRACE << "Won't be logged";
		DE_LOG_DEBUG << "Won't be logged";
		DE_LOG_INFO << "Will log info";
		DE_LOG_WARNING << "Will log warning";
		DE_LOG_ERROR << "Will log error";
		DE_LOG_CRITICAL << "Will log critical";
	}

	const std::string EXPECTED =
		"Will log info\n"
		"Will log warning\n"
		"Will log error\n"
		"Will log critical\n"
		;

	BOOST_CHECK_EQUAL(output.str(), EXPECTED);
}

BOOST_FIXTURE_TEST_CASE(LoggerCategorySetsAppropriateLogger, essentials::test_utils::ResourcesDirFixture) {
	std::istringstream iss(
		"<root-logger>"
		"  <level>info</level>"
		"  <appenders>"
		"	<appender>appender-id</appender>"
		"  </appenders>"
		"</root-logger>"
		"<loggers>"
		"  <logger>"
		"	<id>SPECIFIC</id>"
		"	<level>debug</level>"
		"  </logger>"
		"</loggers>"
		"<appenders>"
		"  <appender>"
		"	<id>appender-id</id>"
		"	<type>dormouse_engine::logger::appender::ConsoleAppender</type>"
		"	<layout>layout-id</layout>"
		"  </appender>"
		"</appenders>"
		"<layouts>"
		"  <layout>"
		"	<id>layout-id</id>"
		"	<type>dormouse_engine::logger::layout::EmptyLayout</type>"
		"  </layout>"
		"</layouts>"
		);

	const auto configurationPath = resourcesDir() / "configuration.xml";
	essentials::test_utils::writeToFile(configurationPath, iss.str());
	GlobalLoggerFactory::instance()->reloadConfiguration(configurationPath);
	essentials::RaiiHelper resetConfiguration([]() { GlobalLoggerFactory::instance()->reloadConfiguration(); });

	std::ostringstream output;

	{
		auto clogBuf = std::clog.rdbuf(output.rdbuf());
		essentials::RaiiHelper clogReset(
			[&]() { std::clog.rdbuf(clogBuf); }
		);

		DE_LOGGER_LOCAL_CATEGORY("GENERAL");

		DE_LOG_DEBUG << "This shan't be logged";

		{
			DE_LOGGER_LOCAL_CATEGORY("SPECIFIC");
			DE_LOG_DEBUG << "This shall be logged";
		}

		DE_LOG_DEBUG << "This shan't be logged also";
	}

	const std::string EXPECTED =
		"This shall be logged\n"
		;

	BOOST_CHECK_EQUAL(output.str(), EXPECTED);
}

namespace fully_fledged_logger_configuration_test {

DE_LOGGER_CATEGORY("DORMOUSE_ENGINE.LOGGER.FUNCTIONAL_TEST");

namespace some_verbose_module {

DE_LOGGER_CATEGORY("DORMOUSE_ENGINE.LOGGER.FUNCTIONAL_TEST.SOME_VERBOSE_MODULE");

void someFunction() {
	DE_LOG_TRACE << "Trace message in verbose module";
}

} // namespace some_verbose_module

BOOST_FIXTURE_TEST_CASE(FullyFledgedLoggerConfiguration, essentials::test_utils::ResourcesDirFixture) {
	std::istringstream iss(
		"<root-logger>\n"
		"  <level>info</level>\n"
		"  <appenders>\n"
		"	<appender>console</appender>\n"
		"  </appenders>\n"
		"</root-logger>\n"
		"<loggers>\n"
		"  <logger>\n"
		"	<id>DORMOUSE_ENGINE.LOGGER.FUNCTIONAL_TEST</id>\n"
		"	<level>debug</level>\n"
		"	<appenders>\n"
		"	  <appender>console</appender>\n"
		"	  <appender>file</appender>\n"
		"	</appenders>\n"
		"  </logger>\n"
		"  <logger>\n"
		"	<id>DORMOUSE_ENGINE.LOGGER.FUNCTIONAL_TEST.SOME_VERBOSE_MODULE</id>\n"
		"	<level>trace</level>\n"
		"  </logger>\n"
		"</loggers>\n"
		"<appenders>\n"
		"  <appender>\n"
		"	<id>console</id>\n"
		"	<level>info</level>\n"
		"	<type>dormouse_engine::logger::appender::ConsoleAppender</type>\n"
		"	<layout>basic</layout>\n"
		"  </appender>\n"
		"  <appender>\n"
		"	<id>file</id>\n"
		"	<level>trace</level>\n"
		"	<type>dormouse_engine::logger::appender::FileAppender</type>\n"
		"	<layout>basic</layout>\n"
		"	<log-file>log</log-file>\n"
		"  </appender>\n"
		"</appenders>\n"
		"<layouts>\n"
		"  <layout>\n"
		"	<id>basic</id>\n"
		"	<type>dormouse_engine::logger::layout::BasicLayout</type>\n"
		"  </layout>\n"
		"  <layout>\n"
		"	<id>detailed</id>\n"
		"	<type>dormouse_engine::logger::layout::DetailedLayout</type>\n"
		"	<configuration>\n"
		"	  <time-precision>milliseconds</time-precision>\n"
		"	</configuration>\n"
		"  </layout>\n"
		"</layouts>\n"
		"<log-files>\n"
		"  <log-file>\n"
		"	<id>log</id>\n"
		"	<path>test-output.log</path>\n"
		"	<overwrite>1</overwrite>\n"
		"  </log-file>\n"
		"</log-files>\n"
		);

	const auto configurationPath = resourcesDir() / "configuration.xml";
	essentials::test_utils::writeToFile(configurationPath, iss.str());
	GlobalLoggerFactory::instance()->reloadConfiguration(configurationPath);
	essentials::RaiiHelper resetConfiguration([]() { GlobalLoggerFactory::instance()->reloadConfiguration(); });

	std::ostringstream consoleOutput;

	{
		auto clogBuf = std::clog.rdbuf(consoleOutput.rdbuf());
		essentials::RaiiHelper clogReset(
			[&]() { std::clog.rdbuf(clogBuf); }
		);

		DE_LOG_DEBUG << "Debug not logged on console, but logged in file";
		DE_LOG_TRACE << "Trace not logged in DORMOUSE_ENGINE.LOGGER.FUNCTIONAL_TEST";
		DE_LOG_INFO << "Info logged everywhere";

		some_verbose_module::someFunction();
	}

	const std::string CONSOLE_EXPECTED =
		"DORMOUSE_ENGINE.LOGGER.FUNCTIONAL_TEST [INFO] - Info logged everywhere\n"
		;

	BOOST_CHECK_EQUAL(consoleOutput.str(), CONSOLE_EXPECTED);

	const std::string FILE_EXPECTED =
		"DORMOUSE_ENGINE.LOGGER.FUNCTIONAL_TEST [DEBUG] - Debug not logged on console, but logged in file\n"
		"DORMOUSE_ENGINE.LOGGER.FUNCTIONAL_TEST [INFO] - Info logged everywhere\n"
		"DORMOUSE_ENGINE.LOGGER.FUNCTIONAL_TEST.SOME_VERBOSE_MODULE [TRACE] - Trace message in verbose module\n"
		;

	BOOST_CHECK_EQUAL(essentials::test_utils::readFile("test-output.log"), FILE_EXPECTED);
}

} // namespace fully_fledged_logger_configuration_test

BOOST_AUTO_TEST_SUITE_END(/* LoggerFunctionalTestSuite */);

} // anonymous namespace
