#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/logger/appender/Appender.hpp"
#include "dormouse-engine/logger/layout/EmptyLayout.hpp"
#include "dormouse-engine/essentials/test-utils/GMockFixture.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;
using namespace dormouse_engine::logger::appender;

namespace /* anonymous */ {

class ConcreteAppender : public Appender {
public:

	ConcreteAppender(Level level, layout::LayoutSharedPtr layout) :
		Appender(level, layout) {
	}

	MOCK_METHOD1(doAppend, void (const std::string&));

};

class ConcreteLayout : public layout::Layout {
public:

	MOCK_METHOD3(format, std::string (Level, const Context&, const std::string&));

};

BOOST_AUTO_TEST_SUITE(LoggerTestSuite);
BOOST_AUTO_TEST_SUITE(AppenderTestSuite);
BOOST_FIXTURE_TEST_SUITE(AppenderTestSuite, essentials::test_utils::GMockFixture);

BOOST_AUTO_TEST_CASE(CallsSubclassDoAppend) {
	const std::string testString("test string");

	layout::LayoutSharedPtr layout(new layout::EmptyLayout);
	ConcreteAppender appender(Level::INFO, layout);

	EXPECT_CALL(appender, doAppend(testString + '\n'));

	appender.append(Level::INFO, Context(), testString);
}

BOOST_AUTO_TEST_CASE(UsesProvidedLayout) {
	const std::string testString("test string");
	const std::string layoutApplicationResult("test string lain out");

	layout::LayoutSharedPtr layout(new ConcreteLayout);
	ConcreteAppender appender(Level::INFO, layout);

	EXPECT_CALL(
			dynamic_cast<ConcreteLayout&>(*layout),
			format(Level(Level::INFO), testing::_, testString)
			).WillOnce(testing::Return(layoutApplicationResult));
	EXPECT_CALL(appender, doAppend(layoutApplicationResult));

	appender.append(Level::INFO, Context(), testString);
}

BOOST_AUTO_TEST_CASE(DoesntLogUnderLevel) {
	const std::string debugTestString("debug test string");
	const std::string infoTestString("info test string");
	const std::string warningTestString("warning test string");

	layout::LayoutSharedPtr layout(new layout::EmptyLayout);
	ConcreteAppender appender(Level::INFO, layout);

	EXPECT_CALL(appender, doAppend(infoTestString + '\n')).Times(1);
	EXPECT_CALL(appender, doAppend(warningTestString + '\n')).Times(1);

	appender.append(Level::DEBUG, Context(), debugTestString);
	appender.append(Level::INFO, Context(), infoTestString);
	appender.append(Level::WARNING, Context(), warningTestString);
}

BOOST_AUTO_TEST_SUITE_END(/* AppenderTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* AppenderTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* LoggerTestSuite */);

} // anonymous namespace
