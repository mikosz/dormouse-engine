#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <string>
#include <sstream>

#include "dormouse-engine/configuration/readers/FlatConfigurationReader.hpp"
#include "dormouse-engine/configuration/parsers/PropertiesParser.hpp"

#include "dormouse-engine/essentials/test-utils/test-utils.hpp"
#include "dormouse-engine/essentials/test-utils/GMockFixture.hpp"

#include "dormouse-engine/system/platform.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::configuration;
using namespace dormouse_engine::configuration::readers;

#if defined(DE_COMPILER_VISUAL_CXX)
#	pragma warning(disable: 4373) // gmock const param warning
#endif /* DE_COMPILER_VISUAL_CXX */

namespace {

class MockIntFloatConfiguration : public Configuration<int, float> {
public:

	MOCK_METHOD0(clear, void ());

	MOCK_CONST_METHOD0(empty, bool ());

	MOCK_CONST_METHOD1(count, size_t (int));

	MOCK_CONST_METHOD1(get, float (int));

	MOCK_CONST_METHOD2(getAll, void (int, Values*));

	MOCK_METHOD2(set, void (int, float));

	MOCK_METHOD2(add, void (int, float));

	MOCK_METHOD1(erase, void (int));

	MOCK_CONST_METHOD1(keys, void (Keys*));

};

BOOST_FIXTURE_TEST_SUITE(SimpleConfigurationReaderTestSuite, essentials::test_utils::GMockFixture);

BOOST_AUTO_TEST_CASE(AddsKeyValuePairsParsedFromStream) {
	const std::string CONFIGURATION_CONTENTS =
			"42 = 3.14\n"
			"0 = 0.001\n"
			;
	std::istringstream configurationStream(CONFIGURATION_CONTENTS);

	testing::StrictMock<MockIntFloatConfiguration> configuration;
	EXPECT_CALL(configuration, add(42, 3.14f)).Times(1);
	EXPECT_CALL(configuration, add(0, 0.001f)).Times(1);

	FlatConfigurationReader<int, float> reader;
	reader.read(parsers::PropertiesParser(), configurationStream, &configuration);

	BOOST_CHECK(true); // to omit the "test did not check any assertions" warning
}

BOOST_FIXTURE_TEST_CASE(AddsKeyValuePairsParsedFromFile, essentials::test_utils::ResourcesDirFixture) {
	const std::string CONFIGURATION_CONTENTS =
			"42 = 3.14\n"
			"0 = 0.001\n"
			;
	const boost::filesystem::path CONFIGURATION_PATH(resourcesDir() / "configuration.properties");
	essentials::test_utils::writeToFile(CONFIGURATION_PATH, CONFIGURATION_CONTENTS);

	testing::StrictMock<MockIntFloatConfiguration> configuration;
	EXPECT_CALL(configuration, add(42, 3.14f)).Times(1);
	EXPECT_CALL(configuration, add(0, 0.001f)).Times(1);

	FlatConfigurationReader<int, float> reader;
	reader.read(parsers::PropertiesParser(), CONFIGURATION_PATH, &configuration);

	BOOST_CHECK(true); // to omit the "test did not check any assertions" warning
}

BOOST_AUTO_TEST_SUITE_END(/* SimpleConfigurationReaderTestSuite */)

} // anonymous namespace
