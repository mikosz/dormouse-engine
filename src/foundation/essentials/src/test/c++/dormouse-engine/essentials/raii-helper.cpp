#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <sstream>
#include <iostream>
#include <stdexcept>
#include <functional>

#include "dormouse-engine/essentials/raii-helper.hpp"

using namespace dormouse_engine;

namespace {

class DummyException : public std::exception {
};

void reset(bool& b, bool v) {
	b = v;
}

BOOST_AUTO_TEST_SUITE(RaiiHelperTestSuite);

BOOST_AUTO_TEST_CASE(basicTest) {
	bool b = false;

	{
		essentials::RaiiHelper helper(std::bind(&reset, std::ref(b), true));
		BOOST_CHECK_EQUAL(b, false);
	}
	BOOST_CHECK_EQUAL(b, true);
}

BOOST_AUTO_TEST_CASE(resetTest) {
	bool b = false;

	{
		essentials::RaiiHelper helper(std::bind(&reset, std::ref(b), true));
		BOOST_CHECK_EQUAL(b, false);
		helper.reset();
	}
	BOOST_CHECK_EQUAL(b, false);
}

BOOST_AUTO_TEST_CASE(exceptionTest) {
	bool b = false;

	try {
		essentials::RaiiHelper helper(std::bind(&reset, std::ref(b), true));
		BOOST_CHECK_EQUAL(b, false);
		throw DummyException();
	} catch (const DummyException&) {
		BOOST_CHECK_EQUAL(b, true);
	}
}

BOOST_AUTO_TEST_SUITE_END();

} // anonymous namespace
