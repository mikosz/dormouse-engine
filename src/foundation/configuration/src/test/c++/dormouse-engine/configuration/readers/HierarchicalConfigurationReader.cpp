#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/essentials/test-utils/test-utils.hpp"
#include "dormouse-engine/configuration/hierarchical/HierarchicalConfiguration.hpp"
#include "dormouse-engine/configuration/parsers/XMLParser.hpp"
#include "dormouse-engine/configuration/readers/HierarchicalConfigurationReader.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::configuration;
using namespace dormouse_engine::configuration::readers;

using dormouse_engine::configuration::hierarchical::HierarchicalConfiguration;
using dormouse_engine::configuration::hierarchical::HierarchicalConfigurationSharedPtr;

namespace {

BOOST_AUTO_TEST_SUITE(CoconutToolsConfigurationTestSuite);
BOOST_AUTO_TEST_SUITE(ReadersTestSuite);
BOOST_AUTO_TEST_SUITE(HierarchicalConfigurationReaderTestSuite);

BOOST_AUTO_TEST_CASE(AddsKeyValuePairsParsedFromStream) {
	const std::string CONFIGURATION_CONTENTS =
			"<parent>\n"
			"  <key0>value 0.0</key0>\n"
			"  <key1>value 0.1</key1>\n"
			"</parent>\n"
			"<parent>\n"
			"  <key0>value 1.0</key0>\n"
			"  <key1>value 1.1</key1>\n"
			"</parent>\n"
			;
	std::istringstream configurationStream(CONFIGURATION_CONTENTS);

	HierarchicalConfigurationSharedPtr configuration = HierarchicalConfiguration::create();

	HierarchicalConfigurationReader reader;
	reader.read(parsers::XMLParser(), configurationStream, configuration.get());

	HierarchicalConfiguration::Values nodes;
	configuration->getAll("parent", &nodes);

	BOOST_CHECK_EQUAL(nodes.size(), 2);
	for (size_t i = 0; i < nodes.size(); ++i) {
		std::ostringstream value1, value2;
		value1 << "value " << i << ".0";
		value2 << "value " << i << ".1";

		BOOST_CHECK_EQUAL(nodes[i]->get("key0")->text(), value1.str());
		BOOST_CHECK_EQUAL(nodes[i]->get("key1")->text(), value2.str());
	}
}

BOOST_FIXTURE_TEST_CASE(AddsKeyValuePairsParsedFromFile, essentials::test_utils::ResourcesDirFixture) {
	const std::string CONFIGURATION_CONTENTS =
			"<parent>\n"
			"  <key0>value 0.0</key0>\n"
			"  <key1>value 0.1</key1>\n"
			"</parent>\n"
			"<parent>\n"
			"  <key0>value 1.0</key0>\n"
			"  <key1>value 1.1</key1>\n"
			"</parent>\n"
			;
	const boost::filesystem::path CONFIGURATION_PATH(resourcesDir() / "configuration.xml");
	essentials::test_utils::writeToFile(CONFIGURATION_PATH, CONFIGURATION_CONTENTS);

	HierarchicalConfigurationSharedPtr configuration = HierarchicalConfiguration::create();

	HierarchicalConfigurationReader reader;
	reader.read(parsers::XMLParser(), CONFIGURATION_PATH, configuration.get());

	HierarchicalConfiguration::Values nodes;
	configuration->getAll("parent", &nodes);

	BOOST_CHECK_EQUAL(nodes.size(), 2);
	for (size_t i = 0; i < nodes.size(); ++i) {
		std::ostringstream value1, value2;
		value1 << "value " << i << ".0";
		value2 << "value " << i << ".1";

		BOOST_CHECK_EQUAL(nodes[i]->get("key0")->text(), value1.str());
		BOOST_CHECK_EQUAL(nodes[i]->get("key1")->text(), value2.str());
	}
}

BOOST_AUTO_TEST_SUITE_END(/* HierarchicalConfigurationReaderTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* ReadersTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* CoconutToolsConfigurationTestSuite */);

} // anonymous namespace
