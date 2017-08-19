#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <sstream>
#include <deque>
#include <functional>
#include <memory>

#include "dormouse-engine/configuration/parsers/XMLParser.hpp"
#include "dormouse-engine/essentials/pointee.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::configuration::parsers;

namespace {

struct ParserCallbackCall {

	virtual ~ParserCallbackCall() {
	}

};

typedef std::deque<std::shared_ptr<ParserCallbackCall> > ExpectedCalls;

struct NewChildCall : ParserCallbackCall {

	std::string node;

	std::string text;

	NewChildCall(const std::string& node, const std::string& text) :
		node(node),
		text(text)
	{
	}

};

void newChild(ExpectedCalls* expectedPtr, const std::string& node, const std::string& text) {
	ExpectedCalls& expected = essentials::pointee(expectedPtr);

	BOOST_REQUIRE(!expected.empty());

	NewChildCall& newChildCall = dynamic_cast<NewChildCall&>(*expected.front());

	BOOST_CHECK_EQUAL(node, newChildCall.node);
	BOOST_CHECK_EQUAL(text, newChildCall.text);

	expected.pop_front();
}

struct ChildEndCall : ParserCallbackCall {
};

void childEnd(ExpectedCalls* expectedPtr) {
	ExpectedCalls& expected = essentials::pointee(expectedPtr);
	BOOST_REQUIRE(!expected.empty());
	expected.pop_front();
}

BOOST_AUTO_TEST_SUITE(CoconutToolsConfigurationTestSuite);
BOOST_AUTO_TEST_SUITE(ParsersTestSuite);
BOOST_AUTO_TEST_SUITE(XMLParserTestSuite);

BOOST_AUTO_TEST_CASE(EmptyConfigurationDoesntCallCallbacks) {
	const std::string CONFIGURATION_CONTENTS = "";
	std::istringstream configurationStream(CONFIGURATION_CONTENTS);

	ExpectedCalls expectedCalls;

	XMLParser parser;
	parser.parse(
			configurationStream,
			std::bind(&newChild, &expectedCalls, std::placeholders::_1, std::placeholders::_2),
			std::bind(&childEnd, &expectedCalls)
	);

	BOOST_CHECK(true); // to omit the "test did not check any assertions" warning
}

BOOST_AUTO_TEST_CASE(ReadsValues) {
	const std::string CONFIGURATION_CONTENTS =
			"<grandparent>\n"
			"  <parent>\n"
			"	<child>\n"
			"	  <key1>value of key 1</key1>\n"
			"	  <key2>value of key 2</key2>\n"
			"	</child>\n"
			"  </parent>\n"
			"</grandparent>\n"
			;
	std::istringstream configurationStream(CONFIGURATION_CONTENTS);

	ExpectedCalls expectedCalls;
	expectedCalls.push_back(std::shared_ptr<ParserCallbackCall>(new NewChildCall("grandparent", std::string())));
	expectedCalls.push_back(std::shared_ptr<ParserCallbackCall>(new NewChildCall("parent", std::string())));
	expectedCalls.push_back(std::shared_ptr<ParserCallbackCall>(new NewChildCall("child", std::string())));
	expectedCalls.push_back(std::shared_ptr<ParserCallbackCall>(new NewChildCall("key1", "value of key 1")));
	expectedCalls.push_back(std::shared_ptr<ParserCallbackCall>(new ChildEndCall));
	expectedCalls.push_back(std::shared_ptr<ParserCallbackCall>(new NewChildCall("key2", "value of key 2")));
	expectedCalls.push_back(std::shared_ptr<ParserCallbackCall>(new ChildEndCall));
	expectedCalls.push_back(std::shared_ptr<ParserCallbackCall>(new ChildEndCall));
	expectedCalls.push_back(std::shared_ptr<ParserCallbackCall>(new ChildEndCall));
	expectedCalls.push_back(std::shared_ptr<ParserCallbackCall>(new ChildEndCall));

	XMLParser parser;
	parser.parse(
			configurationStream,
			std::bind(&newChild, &expectedCalls, std::placeholders::_1, std::placeholders::_2),
			std::bind(&childEnd, &expectedCalls)
	);
}

BOOST_AUTO_TEST_CASE(TrimsValues) {
	const std::string CONFIGURATION_CONTENTS =
			"<child>  \n"
			"\t	  trimmed value\t\t   \n"
			"\n	 "
			"</child>"
			;
	std::istringstream configurationStream(CONFIGURATION_CONTENTS);

	ExpectedCalls expectedCalls;
	expectedCalls.push_back(std::shared_ptr<ParserCallbackCall>(new NewChildCall("child", "trimmed value")));
	expectedCalls.push_back(std::shared_ptr<ParserCallbackCall>(new ChildEndCall));

	XMLParser parser;
	parser.parse(
			configurationStream,
			std::bind(&newChild, &expectedCalls, std::placeholders::_1, std::placeholders::_2),
			std::bind(&childEnd, &expectedCalls)
	);
}

BOOST_AUTO_TEST_SUITE_END(/* XMLParserTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* ParsersTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* CoconutToolsConfigurationTestSuite */);

} // anonymous namespace
