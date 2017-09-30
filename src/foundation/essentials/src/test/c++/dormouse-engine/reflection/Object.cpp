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

struct Complex : public ReflectiveObject<Complex> {

	static constexpr auto CLASS_NAME = "object_test_detail::Complex";

	Complex(std::string s, int i) :
		s(std::move(s)),
		i(i)
	{
	}

	std::string s;
	int i;
};

struct ReflectedObject : public ReflectiveObject<ReflectedObject> {

	static constexpr auto CLASS_NAME = "object_test_detail::ReflectedObject";

	ReflectedObject(int i, Complex complex) :
		i_(i),
		complex_(complex)
	{
	}

	int& i() {
		return i_;
	}

	const Complex& complex() const {
		return complex_;
	}

private:

	int i_;

	Complex complex_;

	friend void declareReflectedObject();

};

void declareComplex() {
	ponder::Class::declare<Complex>(Complex::CLASS_NAME)
		.property("s", &Complex::s)
		.property("i", &Complex::i)
		;
}

void declareReflectedObject() {
	ponder::Class::declare<ReflectedObject>(ReflectedObject::CLASS_NAME)
		.constructor<int, Complex>()
		.property("i", &ReflectedObject::i).tag(Tag::SHADER_PARAMETER)
		.property("complex", &ReflectedObject::complex)
		;
}

const Interface reflectObject([[maybe_unused]] const ReflectedObject& object) {
	return Interface(essentials::make_observer(&ponder::classByType<ReflectedObject>()));
}

} // namespace object_test_detail

PONDER_AUTO_TYPE(object_test_detail::SomeObject, &object_test_detail::declareSomeObject)
PONDER_AUTO_TYPE(object_test_detail::Complex, &object_test_detail::declareComplex);
PONDER_AUTO_TYPE(object_test_detail::ReflectedObject, &object_test_detail::declareReflectedObject);

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(ReflectionSuite);
BOOST_AUTO_TEST_SUITE(ObjectSuite);

BOOST_AUTO_TEST_CASE(CanCreateReflectionObjectsNonintrusively) {
	auto actualObject = object_test_detail::SomeObject();
	auto reflectionObject = Object(essentials::make_observer(&actualObject));
	
	auto iface = reflectionObject.iface();

	BOOST_CHECK_EQUAL(iface.name().string(), object_test_detail::SomeObject::CLASS_NAME);
}

BOOST_AUTO_TEST_CASE(CanCreateReflectionObjectsIntrusively) {
	auto actualObject = object_test_detail::ReflectedObject(42, object_test_detail::Complex("test"s, 666));
	auto reflectionObject = Object(essentials::make_observer(&actualObject));

	auto iface = reflectionObject.iface();

	BOOST_CHECK_EQUAL(iface.name().string(), object_test_detail::ReflectedObject::CLASS_NAME);
}

BOOST_AUTO_TEST_CASE(CanRetrievePropertyValues) {
	const auto actualObject = object_test_detail::ReflectedObject(42, object_test_detail::Complex("test"s, 666));
	const auto constReflectionObject = Object(essentials::make_observer(&actualObject));

	BOOST_CHECK_EQUAL(constReflectionObject.property<int>("i"), 42);

	const auto& complex = constReflectionObject.property<const object_test_detail::Complex&>("complex");

	BOOST_CHECK_EQUAL(complex.i, 666);
	BOOST_CHECK_EQUAL(&complex, &actualObject.complex());
}

BOOST_AUTO_TEST_CASE(CanChangePropertyValues) {
	auto actualObject = object_test_detail::ReflectedObject(42, object_test_detail::Complex("test"s, 666));
	auto reflectionObject = Object(essentials::make_observer(&actualObject));

	// non-const access to const?
	reflectionObject.property<object_test_detail::Complex&>("complex").i = 1;
	reflectionObject.property<int&>("i") = 666;
	//BOOST_CHECK_EQUAL(reflectionObject.property<int>("i"), 666);
}

BOOST_AUTO_TEST_SUITE_END(/* ObjectSuite */);
BOOST_AUTO_TEST_SUITE_END(/* ReflectionSuite */);

} // anonymous namespace
