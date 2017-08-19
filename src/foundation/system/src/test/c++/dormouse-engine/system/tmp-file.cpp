#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <sstream>

#include <boost/filesystem.hpp>

#include "dormouse-engine/system/tmp-file.hpp"

#include "dormouse-engine/essentials/test-utils/test-utils.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::system;

namespace {

BOOST_FIXTURE_TEST_SUITE(TmpFileTestSuite, essentials::test_utils::ResourcesDirFixture);

BOOST_AUTO_TEST_CASE(CreatesTmpFiles) {
	const boost::filesystem::path PATH1(resourcesDir() / "prefix0suffix");
	const boost::filesystem::path PATH2(resourcesDir() / "prefix1suffix");

	if (boost::filesystem::exists(PATH1) || boost::filesystem::exists(PATH2)) {
		BOOST_FAIL("For this test to succeed it is required that neither prefix0suffix nor prefix1suffix exist");
	}
	BOOST_CHECK_EQUAL(createTmpFile((resourcesDir() / "prefix").string(), "suffix"), PATH1);
	BOOST_CHECK(!boost::filesystem::is_directory(PATH1));
	BOOST_CHECK_EQUAL(createTmpFile((resourcesDir() / "prefix").string(), "suffix"), PATH2);
	BOOST_CHECK(!boost::filesystem::is_directory(PATH2));
}

BOOST_AUTO_TEST_CASE(CreatesTmpDirectories) {
	const boost::filesystem::path PATH1(resourcesDir() / "prefix0suffix");
	const boost::filesystem::path PATH2(resourcesDir() / "prefix1suffix");

	if (boost::filesystem::exists(PATH1) || boost::filesystem::exists(PATH2)) {
		BOOST_FAIL("For this test to succeed it is required that neither prefix0suffix nor prefix1suffix exist");
	}
	BOOST_CHECK_EQUAL(createTmpDir((resourcesDir() / "prefix").string(), "suffix"), PATH1);
	BOOST_CHECK(boost::filesystem::is_directory(PATH1));
	BOOST_CHECK_EQUAL(createTmpDir((resourcesDir() / "prefix").string(), "suffix"), PATH2);
	BOOST_CHECK(boost::filesystem::is_directory(PATH2));
}

BOOST_AUTO_TEST_CASE(CreatesTmpDirectoryWhenFileExists) {
	const boost::filesystem::path PATH1(resourcesDir() / "prefix0suffix");
	const boost::filesystem::path PATH2(resourcesDir() / "prefix1suffix");

	if (boost::filesystem::exists(PATH1) || boost::filesystem::exists(PATH2)) {
		BOOST_FAIL("For this test to succeed it is required that neither prefix0suffix nor prefix1suffix exist");
	}

	essentials::test_utils::writeToFile(PATH1, "");
	BOOST_CHECK_EQUAL(createTmpDir((resourcesDir() / "prefix").string(), "suffix"), PATH2);
	BOOST_CHECK(boost::filesystem::is_directory(PATH2));
}

BOOST_AUTO_TEST_SUITE_END();

} // anonymous namespace
