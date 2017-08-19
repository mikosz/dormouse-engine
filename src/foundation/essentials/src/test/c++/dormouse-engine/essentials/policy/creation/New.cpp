#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <type_traits>

#include "dormouse-engine/essentials/policy/creation/New.hpp"

namespace {

using namespace dormouse_engine;
using namespace dormouse_engine::policy;
using namespace dormouse_engine::policy::creation;

struct Object {

	Object() :
		value(-1)
	{
	}

	Object(int value) :
		value(value)
	{
	}

	virtual ~Object() = default;

	int value;

};

struct SubObject : Object {

	enum {
		MULTIPLIER = 47,
	};

	SubObject(int value) :
		Object(value * MULTIPLIER)
	{
	}

};

BOOST_AUTO_TEST_SUITE(PolicyTestSuite);
BOOST_AUTO_TEST_SUITE(NewTestSuite);

BOOST_AUTO_TEST_CASE(CreatesSimpleObject) {
	New<Object> creator;
	auto obj = creator.create();

	BOOST_CHECK_EQUAL(obj->value, -1);
}

BOOST_AUTO_TEST_CASE(PassesConstructorParameters) {
	enum {
		VALUE = 42,
	};

	New<Object, int> creator;
	auto obj = creator.create(VALUE);

	BOOST_CHECK_EQUAL(obj->value, VALUE);
}

BOOST_AUTO_TEST_CASE(CreatesSubtypeObjects) {
	enum {
		VALUE = 42,
		MULTIPLIED_VALUE = VALUE * SubObject::MULTIPLIER,
	};

	New<Object, int> creator = New<Object, int>::makeCreator<SubObject>();
	auto obj = creator.create(VALUE);

	BOOST_CHECK_EQUAL(obj->value, MULTIPLIED_VALUE);
}

BOOST_AUTO_TEST_SUITE_END(/* NewTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PolicyTestSuite */);

} // anonymous namespace
