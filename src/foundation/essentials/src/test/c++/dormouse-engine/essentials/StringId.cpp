#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/essentials/StringId.hpp"

namespace /* anonymous */ {

using namespace dormouse_engine::essentials;

BOOST_AUTO_TEST_SUITE(StringIdTestSuite);

BOOST_AUTO_TEST_CASE(StoredStringsCanBeRetrieved) {
	const auto qwertyId = Strings::instance()->add("qwerty");
	const auto azertyId = Strings::instance()->add("azerty");

	BOOST_CHECK_EQUAL(qwertyId.string(), "qwerty");
	BOOST_CHECK_EQUAL(azertyId.string(), "azerty");

	Strings::setInstance(nullptr);
}

BOOST_AUTO_TEST_CASE(StoringSameStringReturnsSameHash) {
	const auto qwertyId = Strings::instance()->add("qwerty");
	const auto qwerty2Id = Strings::instance()->add("qwerty");

	BOOST_CHECK(qwertyId == qwerty2Id);
	BOOST_CHECK_EQUAL(qwertyId.string(), "qwerty");

	Strings::setInstance(nullptr);
}

BOOST_AUTO_TEST_CASE(StringIdIsConstructibleFromString) {
	const auto fromString = StringId("from string");

	BOOST_CHECK_EQUAL(fromString.string(), "from string");
}

BOOST_AUTO_TEST_SUITE_END(/* StringIdTestSuite */);

} // anonymous namespace
