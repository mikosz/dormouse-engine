#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#pragma warning(push, 3)
#	include <ponder/classbuilder.hpp>
#	include <ponder/uses/runtime.hpp>
#pragma warning(pop)

#include "dormouse-engine/reflection/Object.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::reflection;
using namespace std::string_literals;

namespace test_detail {

struct SomeObject {
};

void declareSomeObject() {
	ponder::Class::declare<SomeObject>("test_detail::SomeObject");
}

const Interface reflectObject([[maybe_unused]] const SomeObject& object) {
	return Interface(essentials::make_observer(&ponder::classByType<SomeObject>()));
}

struct DerivedObject : ReflectiveObject {

	DerivedObject() :
		ReflectiveObject("test_detail::DerivedObject")
	{
	}

};

void declareDerivedObject() {
	ponder::Class::declare<DerivedObject>("test_detail::DerivedObject");
}

} // namespace test_detail

PONDER_AUTO_TYPE(test_detail::SomeObject, &test_detail::declareSomeObject)
PONDER_AUTO_TYPE(test_detail::DerivedObject, &test_detail::declareDerivedObject)

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(ReflectionInterfaceTestSuite);

BOOST_AUTO_TEST_CASE(CanCreateReflectionObjectsNonintrusively) {
	auto actualObject = test_detail::SomeObject();
	auto reflectionObject = Object(essentials::make_observer(&actualObject));
	
	auto iface = reflectionObject.reflect();

	BOOST_CHECK_EQUAL(iface.name().string(), "test_detail::SomeObject"s);
}

BOOST_AUTO_TEST_CASE(CanCreateReflectionObjectsIntrusively) {
	auto actualObject = test_detail::DerivedObject();
	auto reflectionObject = Object(essentials::make_observer(&actualObject));

	auto iface = reflectionObject.reflect();

	BOOST_CHECK_EQUAL(iface.name().string(), "test_detail::DerivedObject"s);
}

BOOST_AUTO_TEST_SUITE_END(/* ReflectionInterfaceTestSuite */);

} // anonymous namespace
