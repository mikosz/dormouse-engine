#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <iostream>
#include <sstream>
#include <functional>

#include "dormouse-engine/essentials/raii-helper.hpp"
#include "dormouse-engine/configuration/readers/HierarchicalConfigurationReader.hpp"
#include "dormouse-engine/configuration/parsers/XMLParser.hpp"
#include "dormouse-engine/logger/LoggerFactory.hpp"
#include "dormouse-engine/logger/SystemLogger.hpp"
#include "dormouse-engine/logger/macros.hpp"
#include "dormouse-engine/logger.hpp"
#include "dormouse-engine/logger/appender/DebugWindowAppender.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;

namespace {

BOOST_AUTO_TEST_SUITE(LoggerFunctionalTestSuite);

BOOST_AUTO_TEST_CASE(DefaultConfigurationPrintsInfoToStdlog) {
	Logger& logger = *SystemLogger::instance();
	
	std::ostringstream output;

	{
		auto clogBuf = std::clog.rdbuf(output.rdbuf());
		essentials::RaiiHelper clogReset(
			[&]() { std::clog.rdbuf(clogBuf); }
			);

		// Example of API logging using the log function
		logger.log(Level::TRACE) << "Trace level hidden";
		logger.log(Level::DEBUG) << "Debug level hidden";
		logger.log(Level::INFO) << "Log on level info with implicit context";
		logger.log(Level::WARNING, Context()) << "Log on level info with default context";
		logger.log(Level::ERROR, Context("category", "file", 123, "function")) << "Log on level error with full context";
		{
			DE_LOGGER_LOCAL_CATEGORY("category");
			logger.log(Level::CRITICAL, DE_LOGGER_CONTEXT) << "Log on level critical with context from macro";
		}

		// Example of API logging using level specific functions
		logger.trace() << "Trace level hidden";
		logger.debug() << "Debug level hidden";
		logger.info() << "Log on level info with implicit context";
		logger.warning(Context()) << "Log on level info with default context";
		logger.error(Context("category", "file", 123, "function")) << "Log on level error with full context";
		{
			DE_LOGGER_LOCAL_CATEGORY("category");
			logger.critical(DE_LOGGER_CONTEXT) << "Log on level critical with context from macro";
		}
	}

	const std::string EXPECTED =
			"Log on level info with implicit context\n"
			"Log on level info with default context\n"
			"Log on level error with full context\n"
			"Log on level critical with context from macro\n"
			;

	BOOST_CHECK_EQUAL(output.str(), EXPECTED + EXPECTED);
}

BOOST_AUTO_TEST_CASE(LoggerFactoryProducesConfiguredLoggers) {
	auto configurationData = dormouse_engine::configuration::hierarchical::HierarchicalConfiguration::create();

	std::istringstream iss(
		"<root-logger>"
		"  <level>error</level>"
		"  <appenders>"
		"	<appender>appender-id</appender>"
		"  </apenders>"
		"</root-logger>"
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

	dormouse_engine::configuration::readers::HierarchicalConfigurationReader().read(
		dormouse_engine::configuration::parsers::XMLParser(),
		iss,
		configurationData.get()
		);

	log_file::LogFileFactorySharedPtr logFileFactory(new log_file::LogFileFactory);

	logger::configuration::LoggerConfigurationSharedPtr config(
		std::make_shared<logger::configuration::LoggerConfiguration>(configurationData, logFileFactory));

	LoggerFactory loggerFactory(config);

	std::ostringstream output;

	{
		auto clogBuf = std::clog.rdbuf(output.rdbuf());
		essentials::RaiiHelper clogReset(
			[&]() { std::clog.rdbuf(clogBuf); }
		);

		Logger& logger = *loggerFactory.create("id");

		// Example of API logging using level specific functions
		logger.trace() << "Trace level hidden";
		logger.debug() << "Debug level hidden";
		logger.info() << "Info level hidden";
		logger.warning() << "Warning level hidden";
		logger.error() << "Log on level error with implicit context";
		logger.critical() << "Log on level critical with implicit context";
	}

	const std::string EXPECTED =
		"Log on level error with implicit context\n"
		"Log on level critical with implicit context\n"
		;

	BOOST_CHECK_EQUAL(output.str(), EXPECTED);
}

BOOST_AUTO_TEST_SUITE_END(/* LoggerFunctionalTestSuite */);

} // anonymous namespace
