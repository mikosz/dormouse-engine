#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/logger/log-file/LogFileFactory.hpp"

#include "dormouse-engine/essentials/test-utils/test-utils.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;
using namespace dormouse_engine::logger::log_file;

namespace {

BOOST_AUTO_TEST_SUITE(LoggerTestSuite);
BOOST_AUTO_TEST_SUITE(LogFileTestSuite);
BOOST_FIXTURE_TEST_SUITE(LogFileFactoryTestSuite, essentials::test_utils::ResourcesDirFixture);

BOOST_AUTO_TEST_CASE(ProducesSameFileWithOverwrite) {
	LogFileFactory factory;
	factory.registerLogFile("to-overwrite", resourcesDir() / "to-overwrite.txt", true);

	essentials::test_utils::writeToFile(resourcesDir() / "to-overwrite.txt", "original content\n");

	{
		LogFileSharedPtr logFile = factory.create("to-overwrite");
		logFile->write("new content\n");
	}

	{
		LogFileSharedPtr logFile = factory.create("to-overwrite");
		logFile->write("more new content\n");
	}

	BOOST_CHECK_EQUAL(
		essentials::test_utils::readFile(resourcesDir() / "to-overwrite.txt"),
		"new content\n"
		"more new content\n"
		);
}

BOOST_AUTO_TEST_CASE(ProducesSameFileWithAppend) {
	LogFileFactory factory;
	factory.registerLogFile("to-append", resourcesDir() / "to-append.txt", false);

	essentials::test_utils::writeToFile(resourcesDir() / "to-append.txt", "original content\n");

	{
		LogFileSharedPtr logFile = factory.create("to-append");
		logFile->write("new content\n");
	}

	{
		LogFileSharedPtr logFile = factory.create("to-append");
		logFile->write("more new content\n");
	}

	BOOST_CHECK_EQUAL(
		essentials::test_utils::readFile(resourcesDir() / "to-append.txt"),
		"original content\n"
		"new content\n"
		"more new content\n"
		);
}

BOOST_AUTO_TEST_SUITE_END(/* LogFileFactoryTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* LogFileTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* LoggerTestSuite */);

} // anonymous namespace
