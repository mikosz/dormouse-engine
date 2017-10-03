#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#pragma warning(push, 3)
#	include <ponder/classbuilder.hpp>
#	include <ponder/uses/runtime.hpp>
#	include <ponder/valuevisitor.hpp>
#pragma warning(pop)

#include "dormouse-engine/reflection/Object.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::reflection;
using namespace std::string_literals;

namespace object_test_detail {

struct NonintrusivelyReflectiveClass {
	int i;
};

constexpr auto NONINTRUSIVE_CLASS_NAME = "object_test_detail::NonintrusivelyReflectiveClass";

void declareNonintrusive() {
	ponder::Class::declare<NonintrusivelyReflectiveClass>(NONINTRUSIVE_CLASS_NAME)
		.property("i", &NonintrusivelyReflectiveClass::i)
		;
}

// TODO: bad names if ReflectiveObject keeps being removed
class IntrusivelyReflectiveClass {
public:

	static constexpr auto CLASS_NAME = "object_test_detail::IntrusivelyReflectiveClass";

	IntrusivelyReflectiveClass(std::string s, int i, int ni) :
		s_(std::move(s)),
		i_(i)
	{
		nonintrusive_.i = ni;
	}

	const std::string& s() const {
		return s_;
	}

	int i() const {
		return i_;
	}

private:

	std::string s_;

	int i_;

	NonintrusivelyReflectiveClass nonintrusive_;

};

void declareIntrusive() {
	ponder::Class::declare<IntrusivelyReflectiveClass>(IntrusivelyReflectiveClass::CLASS_NAME)
		.tag(ClassTag::SERIALISABLE)
		.property("s", &IntrusivelyReflectiveClass::s).tag(PropertyTag::SHADER_PARAMETER)
		.property("i", &IntrusivelyReflectiveClass::i)
		;
}

struct Nonreflective {
};

} // namespace object_test_detail

PONDER_AUTO_TYPE(object_test_detail::NonintrusivelyReflectiveClass, &object_test_detail::declareNonintrusive);
PONDER_AUTO_TYPE(object_test_detail::IntrusivelyReflectiveClass, &object_test_detail::declareIntrusive);

namespace /* anonymous */ {

class Visitor : public ponder::ValueVisitor<int> {
public:
	
	int operator()([[maybe_unused]] bool b) {
		return 0;
	}

	int operator()([[maybe_unused]] long l) {
		return 0;
	}

	int operator()([[maybe_unused]] double d) {
		return 0;
	}

	int operator()([[maybe_unused]] const ponder::String& s) {
		return 0;
	}

	int operator()([[maybe_unused]] const ponder::EnumObject& eo) {
		return 0;
	}

	int operator()([[maybe_unused]] const ponder::UserObject& uo) {
		return 0;
	}

};

BOOST_AUTO_TEST_SUITE(ReflectionSuite);
BOOST_AUTO_TEST_SUITE(ObjectSuite);

// Implement a common behaviour for all reflective classes
void behaviour(Object o) {
	const auto& metaclass = o.metaclass();

	// Behaviour may be driven by class tags...
	if (metaclass.name() == object_test_detail::NONINTRUSIVE_CLASS_NAME) {
		BOOST_CHECK(!metaclass.hasTag(ClassTag::SERIALISABLE));
	} else if (metaclass.name() == object_test_detail::IntrusivelyReflectiveClass::CLASS_NAME) {
		BOOST_CHECK(metaclass.hasTag(ClassTag::SERIALISABLE));
	} else {
		BOOST_FAIL("Unexpected class name: " + metaclass.name());
	}

	auto propertyIt = metaclass.propertyIterator();

	for (const auto& propertyEntry : propertyIt) {
		const auto& property = *propertyEntry.second;

		const auto value = property.get(o.metaobject());

		// ... and property tags
		if (property.name() == "s") {
			BOOST_CHECK(property.hasTag(PropertyTag::SHADER_PARAMETER));
			BOOST_CHECK_EQUAL(property.kind(), ponder::ValueKind::String);
		} else {
			BOOST_CHECK(!property.hasTag(PropertyTag::SHADER_PARAMETER));
		}

		value.visit(Visitor());
	}
}

void behaviour([[maybe_unused]] const object_test_detail::Nonreflective& nonreflective) {
}

BOOST_AUTO_TEST_CASE(UseCase_CreatingCommonBehaviourForAllReflectiveObjects) {
	auto intrusive = object_test_detail::IntrusivelyReflectiveClass("test string", 42, 666);
	auto nonintrusive = object_test_detail::NonintrusivelyReflectiveClass{ 123 };
	auto nonreflective = object_test_detail::Nonreflective();

	behaviour(essentials::make_observer(&intrusive));
	behaviour(essentials::make_observer(&nonintrusive));
	behaviour(nonreflective);
}

BOOST_AUTO_TEST_SUITE_END(/* ObjectSuite */);
BOOST_AUTO_TEST_SUITE_END(/* ReflectionSuite */);

} // anonymous namespace
