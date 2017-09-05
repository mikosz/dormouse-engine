#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#pragma warning(push, 3)
#include <ponder/uses/report.hpp>
#include <ponder/detail/format.hpp>
using namespace ponder::detail::fmt;

#include <ponder/classbuilder.hpp>

#define PONDER_USES_RUNTIME_IMPL
#include <ponder/uses/runtime.hpp>


#include <ponder/uses/report.hpp>
#include <ponder/detail/format.hpp>
using namespace ponder::detail::fmt;

#include <ponder/classbuilder.hpp>
#include <ponder/pondertype.hpp>
#include <ponder/uses/runtime.hpp>
#pragma warning(pop)

#include "dormouse-engine/reflection/Interface.hpp"

using namespace dormouse_engine::reflection;
using namespace std::string_literals;

struct ReflectedObject {

	int foo() {
		return 42;
	}

	std::string text = "To be, or not to be!";

};

void declareReflectedObject() {
	ponder::Class::declare<ReflectedObject>("ReflectedObject")
		.constructor()
		.function("foo", &ReflectedObject::foo)
		.property("text", &ReflectedObject::text)
		;
}

PONDER_AUTO_TYPE(ReflectedObject, &declareReflectedObject)

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(ReflectionInterfaceTestSuite);

BOOST_AUTO_TEST_CASE(CanCallMemberFunctions) {
	const auto& clazz = ponder::classByType<ReflectedObject>();
	const auto factory = ponder::runtime::ObjectFactory(clazz);
	const auto obj = factory.create();

	BOOST_CHECK_EQUAL(ponder::runtime::call(clazz.function("foo"), obj), 42);
	BOOST_CHECK_EQUAL(obj.get("text").to<std::string>(), "To be, or not to be!"s);
}

BOOST_AUTO_TEST_SUITE_END(/* ReflectionInterfaceTestSuite */);

} // anonymous namespace
