#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <sstream>

#include "dormouse-engine/essentials/for-each-in-tuple.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::essentials;

namespace {

struct Appender {

	Appender(std::string& s) :
		s(s)
	{
	}

	template <class T>
	void operator()(T&& v) {
		std::ostringstream oss;
		oss << v;
		s += oss.str();
	}

	std::string& s;

};

BOOST_AUTO_TEST_SUITE(ForEachInTupleTestSuite);

BOOST_AUTO_TEST_CASE(iteratesOverTupleElements) {
	std::string s;
	
	std::tuple<std::string, size_t, std::string> t{ "The answer is ", 42, "!" };
	Appender app(s);

	forEachInTuple(t, app);

	BOOST_CHECK_EQUAL(s, std::string("The answer is 42!"));
}

BOOST_AUTO_TEST_SUITE_END(/* ForEachInTupleTestSuite */);

} // anonymous namespace
