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
	float f;
};

constexpr auto NONINTRUSIVE_CLASS_NAME = "object_test_detail::NonintrusivelyReflectiveClass";

void declareNonintrusive() {
	ponder::Class::declare<NonintrusivelyReflectiveClass>(NONINTRUSIVE_CLASS_NAME)
		.property("f", &NonintrusivelyReflectiveClass::f)
		;
}

// TODO: bad names if ReflectiveObject keeps being removed
class IntrusivelyReflectiveClass {
public:

	static constexpr auto CLASS_NAME = "object_test_detail::IntrusivelyReflectiveClass";

	IntrusivelyReflectiveClass(std::string s, int i, float nf) :
		s_(std::move(s)),
		i_(i)
	{
		nonintrusive_.f = nf;
	}

	const std::string& s() const {
		return s_;
	}

	int i() const {
		return i_;
	}

	const NonintrusivelyReflectiveClass& ni() const {
		return nonintrusive_;
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
		.property("ni", &IntrusivelyReflectiveClass::ni)
		;
}

struct Nonreflective {
};

} // namespace object_test_detail

PONDER_AUTO_TYPE(object_test_detail::NonintrusivelyReflectiveClass, &object_test_detail::declareNonintrusive);
PONDER_AUTO_TYPE(object_test_detail::IntrusivelyReflectiveClass, &object_test_detail::declareIntrusive);

namespace /* anonymous */ {

class Visitor : public ponder::ValueVisitor<void> {
public:
	
	Visitor(double expectedD, const void* expectedObjectPtr) :
		expectedD_(expectedD),
		expectedObjectPtr_(expectedObjectPtr)
	{
	}

	void operator()([[maybe_unused]] bool b) {
		BOOST_FAIL("Unexpected call");
	}

	void operator()([[maybe_unused]] long l) {
		BOOST_CHECK_EQUAL(l, 42);
	}

	void operator()([[maybe_unused]] double d) {
		BOOST_CHECK_CLOSE(d, expectedD_, 0.01f);
	}

	void operator()([[maybe_unused]] ponder::NoType s) {
		BOOST_FAIL("Unexpected call");
	}

	void operator()([[maybe_unused]] const ponder::String& s) {
		BOOST_CHECK_EQUAL(s, "test string");
	}

	void operator()([[maybe_unused]] const ponder::EnumObject& eo) {
		BOOST_FAIL("Unexpected call");
	}

	void operator()([[maybe_unused]] const ponder::UserObject& uo) {
		BOOST_CHECK_EQUAL(uo.getClass().name(), object_test_detail::NONINTRUSIVE_CLASS_NAME);
		BOOST_CHECK_EQUAL(uo.pointer(), expectedObjectPtr_);

		auto propertyIt = uo.getClass().propertyIterator();
		for (const auto& propertyEntry : propertyIt) {
			const auto& property = *propertyEntry.second;
			property.get(uo).visit(Visitor(6.48, nullptr));
		}
	}

private:

	double expectedD_;

	const void* expectedObjectPtr_;

};

BOOST_AUTO_TEST_SUITE(ReflectionSuite);
BOOST_AUTO_TEST_SUITE(ObjectSuite);

// Implement a common behaviour for all reflective classes
void behaviour(Object o, const void* expectedObjectPtr = nullptr) {
	const auto& metaclass = o.metaclass();

	// Behaviour may be driven by class tags
	if (metaclass.name() == object_test_detail::NONINTRUSIVE_CLASS_NAME) {
		BOOST_CHECK(!metaclass.hasTag(ClassTag::SERIALISABLE));
	} else if (metaclass.name() == object_test_detail::IntrusivelyReflectiveClass::CLASS_NAME) {
		BOOST_CHECK(metaclass.hasTag(ClassTag::SERIALISABLE));
	} else {
		BOOST_FAIL("Unexpected class name: " + metaclass.name());
	}

	auto propertyIt = metaclass.propertyIterator();

	// Can iterate over properties...
	for (const auto& propertyEntry : propertyIt) {
		const auto& property = *propertyEntry.second;

		const auto value = property.get(o.metaobject());

		if (property.name() == "s") {
			// Behaviour may be driven by property tags
			BOOST_CHECK(property.hasTag(PropertyTag::SHADER_PARAMETER));
			BOOST_CHECK_EQUAL(property.kind(), ponder::ValueKind::String);
		} else {
			BOOST_CHECK(!property.hasTag(PropertyTag::SHADER_PARAMETER));
		}
		
		// And visit them
		value.visit(Visitor(3.14, expectedObjectPtr));
	}
}

void behaviour([[maybe_unused]] const object_test_detail::Nonreflective& nonreflective) {
}

BOOST_AUTO_TEST_CASE(UseCase_CreatingCommonBehaviourForAllReflectiveObjects) {
	auto intrusive = object_test_detail::IntrusivelyReflectiveClass("test string", 42, 6.48f);
	auto nonintrusive = object_test_detail::NonintrusivelyReflectiveClass{ 3.14f };
	auto nonreflective = object_test_detail::Nonreflective();

	behaviour(essentials::make_observer(&intrusive), &intrusive.ni());
	behaviour(essentials::make_observer(&nonintrusive));
	behaviour(nonreflective);
}

BOOST_AUTO_TEST_SUITE_END(/* ObjectSuite */);
BOOST_AUTO_TEST_SUITE_END(/* ReflectionSuite */);

} // anonymous namespace
