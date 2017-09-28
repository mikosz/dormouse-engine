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

namespace object_test_detail {

struct SomeObject {
	static constexpr auto CLASS_NAME = "object_test_detail::SomeObject";
};

void declareSomeObject() {
	ponder::Class::declare<SomeObject>(SomeObject::CLASS_NAME);
}

const Interface objectInterface([[maybe_unused]] const SomeObject& object) {
	return Interface(essentials::make_observer(&ponder::classByType<SomeObject>()));
}

struct DerivedObject : ReflectiveObject<DerivedObject> {
	static constexpr auto CLASS_NAME = "object_test_detail::DerivedObject";
};

void declareDerivedObject() {
	ponder::Class::declare<DerivedObject>(DerivedObject::CLASS_NAME);
}

static_assert(sizeof(DerivedObject) == sizeof(SomeObject));

} // namespace object_test_detail

PONDER_AUTO_TYPE(object_test_detail::SomeObject, &object_test_detail::declareSomeObject)
PONDER_AUTO_TYPE(object_test_detail::DerivedObject, &object_test_detail::declareDerivedObject)

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(ReflectionInterfaceTestSuite);

BOOST_AUTO_TEST_CASE(CanCreateReflectionObjectsNonintrusively) {
	auto actualObject = object_test_detail::SomeObject();
	auto reflectionObject = Object(essentials::make_observer(&actualObject));
	
	auto iface = reflectionObject.iface();

	BOOST_CHECK_EQUAL(iface.name().string(), object_test_detail::SomeObject::CLASS_NAME);
}

BOOST_AUTO_TEST_CASE(CanCreateReflectionObjectsIntrusively) {
	auto actualObject = object_test_detail::DerivedObject();
	auto reflectionObject = Object(essentials::make_observer(&actualObject));

	auto iface = reflectionObject.iface();

	BOOST_CHECK_EQUAL(iface.name().string(), object_test_detail::DerivedObject::CLASS_NAME);
}

BOOST_AUTO_TEST_SUITE_END(/* ReflectionInterfaceTestSuite */);

} // anonymous namespace
