#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/logger/Level.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(LoggerTestSuite);
BOOST_AUTO_TEST_SUITE(LevelTestSuite);

BOOST_AUTO_TEST_CASE(LevelsCanBeReadFromStream) {
	std::istringstream iss("TRACE DEBUG INFO WarninG ERROR CRITICAL");

	Level level;

	iss >> level;
	BOOST_CHECK_EQUAL(level, Level::TRACE);

	iss >> level;
	BOOST_CHECK_EQUAL(level, Level::DEBUG);

	iss >> level;
	BOOST_CHECK_EQUAL(level, Level::INFO);

	iss >> level;
	BOOST_CHECK_EQUAL(level, Level::WARNING);

	iss >> level;
	BOOST_CHECK_EQUAL(level, Level::ERROR);

	iss >> level;
	BOOST_CHECK_EQUAL(level, Level::CRITICAL);

	BOOST_CHECK(!iss.fail());
}

BOOST_AUTO_TEST_CASE(ReadingLevelsSetIstreamToFailOnErrors) {
	std::istringstream iss("BAD");

	Level level;

	iss >> level;

	BOOST_CHECK(iss.fail());
}

BOOST_AUTO_TEST_CASE(LevelsCanBeWrittenToStream) {
	std::ostringstream oss;

	oss << Level::TRACE << ' '
		<< Level::DEBUG << ' '
		<< Level::INFO << ' '
		<< Level::WARNING << ' '
		<< Level::ERROR << ' '
		<< Level::CRITICAL
		;

	const std::string EXPECTED = "TRACE DEBUG INFO WARNING ERROR CRITICAL";

	BOOST_CHECK_EQUAL(oss.str(), EXPECTED);
}

BOOST_AUTO_TEST_SUITE_END(/* LevelTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* LoggerTestSuite */);

} // anonymous namespace
