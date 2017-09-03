#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <ponder/>

#include "dormouse-engine/reflection/Interface.hpp"

using namespace dormouse_engine::reflection;

namespace /* anonymous */ {

struct ReflectedObject {

	int foo() {
		return 42;
	}

	std::string text = "To be, or not to be!";

};

BOOST_AUTO_TEST_SUITE(ReflectionInterfaceTestSuite);

BOOST_AUTO_TEST_CASE(CanCallMemberFunctions) {
	
}

BOOST_AUTO_TEST_SUITE_END(/* ReflectionInterfaceTestSuite */);

} // anonymous namespace
