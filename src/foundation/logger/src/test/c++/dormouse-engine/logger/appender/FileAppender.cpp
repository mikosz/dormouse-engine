#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <sstream>
#include <iostream>

#include <boost/bind.hpp>

#include "dormouse-engine/logger/appender/FileAppender.hpp"
#include "dormouse-engine/logger/layout/EmptyLayout.hpp"
#include "dormouse-engine/logger/log-file/LogFile.hpp"

#include "dormouse-engine/essentials/test-utils/GMockFixture.hpp"
#include "dormouse-engine/essentials/test-utils/test-utils.hpp"

#include "dormouse-engine/essentials/raii-helper.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;
using namespace dormouse_engine::logger::appender;

namespace {

class MockLogFile : public log_file::LogFile {
public:

	MockLogFile(const boost::filesystem::path& path) :
		log_file::LogFile(path, true) {
	}

	MOCK_METHOD1(write, void (const std::string& message));

};

BOOST_AUTO_TEST_SUITE(LoggerTestSuite);
BOOST_AUTO_TEST_SUITE(AppenderTestSuite);
BOOST_FIXTURE_TEST_SUITE(FileAppenderTestSuite, essentials::test_utils::GMockFixture);

BOOST_FIXTURE_TEST_CASE(WritesToFile, essentials::test_utils::ResourcesDirFixture) {
	const std::string testString("test string");

	layout::LayoutSharedPtr layout(new layout::EmptyLayout);
	log_file::LogFileSharedPtr logFile(new MockLogFile(resourcesDir() / "log-file.txt"));
	FileAppender appender(Level::TRACE, layout, logFile);

	EXPECT_CALL(
			dynamic_cast<MockLogFile&>(*logFile),
			write(layout->format(Level(Level::INFO), Context(), testString))
			);

	appender.append(Level::INFO, Context(), testString);
}

BOOST_AUTO_TEST_SUITE_END(/* FileAppenderTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* AppenderTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* LoggerTestSuite */);

} // anonymous namespace
