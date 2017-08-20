#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <boost/filesystem/path.hpp>

#include "dormouse-engine/essentials/raii-helper.hpp"
#include "dormouse-engine/logger/macros.hpp"
#include "dormouse-engine/essentials/test-utils/test-utils.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;

namespace /* anonymous */ {

class CategoryPrintingLayout : public layout::Layout {
public:

	std::string format(Level, const Context& context, const std::string&) override {
		return context.category + "\n";
	}

};

void createCategoryPrintingLoggerConfigurationFile(const boost::filesystem::path& path) {
	static const essentials::Null CATEGORY_PRINTING_LAYOUT_REGISTRAR =
		GlobalLoggerFactory::instance()->appenderFactory().layoutFactory().registerType<CategoryPrintingLayout>("CategoryPrintingLayout");

	essentials::test_utils::writeToFile(
		path,
		"<root-logger>"
		"  <level>info</level>"
		"  <appenders>"
		"	<appender>appender-id</appender>"
		"  </appenders>"
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
		"	<type>CategoryPrintingLayout</type>"
		"  </layout>"
		"</layouts>"
		);
}

std::string expressionFailsBoostTestCheck() {
	BOOST_FAIL("An unwanted expression was evaluated");
	return "";
}

BOOST_AUTO_TEST_SUITE(LoggerTestSuite);
BOOST_AUTO_TEST_SUITE(MacrosTestSuite);

namespace external_context {

DE_LOGGER_CATEGORY("EXTERNAL");

namespace local_context {

DE_LOGGER_CATEGORY("LOCAL");

BOOST_FIXTURE_TEST_CASE(LogsUsingCategorySpecifiedInLocalContext, essentials::test_utils::ResourcesDirFixture) {
	createCategoryPrintingLoggerConfigurationFile(resourcesDir() / "dormouse-engine-logger.cfg.xml");
	GlobalLoggerFactory::instance()->reloadConfiguration(resourcesDir() / "dormouse-engine-logger.cfg.xml");
	essentials::RaiiHelper configurationResetter([]() { GlobalLoggerFactory::instance()->reloadConfiguration(); });

	std::ostringstream output;

	{
		auto clogRdbuf = std::clog.rdbuf(output.rdbuf());
		essentials::RaiiHelper clogReset(
			[&]() { std::clog.rdbuf(clogRdbuf); }
		);

		DE_LOG_INFO << "";

		BOOST_CHECK_EQUAL(output.str(),
			"LOCAL\n"
			);
	}
}

BOOST_AUTO_TEST_CASE(LogsUsingCategorySpecifiedInScopedContext) {
	DE_LOGGER_LOCAL_CATEGORY("INTERNAL");
}

} // namespace external_context

} // namespace local_context

BOOST_AUTO_TEST_CASE(ExpressionsInLoggingInstructionsAreNotEvaluatedIfLevelUnderLoggerLevel) {
	BOOST_ASSERT((DE_LOGGER)->getLevel() == Level::INFO);

	std::ostringstream output;

	{
		auto clogRdbuf = std::clog.rdbuf(output.rdbuf());
		essentials::RaiiHelper clogReset(
			[&]() { std::clog.rdbuf(clogRdbuf); }
		);

		DE_LOG_TRACE << expressionFailsBoostTestCheck();
		DE_LOG_DEBUG << expressionFailsBoostTestCheck();
		DE_LOG_INFO << "info";
		DE_LOG_WARNING << "warning";
		DE_LOG_ERROR << "error";
		DE_LOG_CRITICAL << "critical";

		BOOST_CHECK_EQUAL(output.str(),
			"info\n"
			"warning\n"
			"error\n"
			"critical\n"
			);
	}
}

BOOST_AUTO_TEST_SUITE_END(/* MacrosTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* LoggerTestSuite */);

} // anonymous namespace
