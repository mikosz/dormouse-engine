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

struct Complex {
	std::string s;
	int* ptr;
};

struct ReflectedObject {

	ReflectedObject(int i, Complex complex) :
		i_(i),
		complex_(complex)
	{
	}

	int i() const {
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

//void declareComplex() {
//	ponder::Class::declare<Complex>("test_detail::Complex")
//		.property("s", &Complex::s)
//		.property("ptr", &Complex::ptr)
//		;
//}

void declareReflectedObject() {
	ponder::Class::declare<ReflectedObject>("test_detail::ReflectedObject")
		//.constructor<int, Complex>()
		.function("i", &ReflectedObject::i)
		//.property("complex", &ReflectedObject::complex)
		;
}

const Interface reflectObject([[maybe_unused]] const ReflectedObject& object) {
	return Interface(essentials::make_observer(&ponder::classByType<ReflectedObject>()));
}

} // namespace test_detail

//PONDER_AUTO_TYPE(test_detail::Complex, &test_detail::declareComplex)
PONDER_AUTO_TYPE(test_detail::ReflectedObject, &test_detail::declareReflectedObject)

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(ReflectionInterfaceTestSuite);

BOOST_AUTO_TEST_CASE(CanCallMemberFunctions) {
	auto i = 666;
	auto actualObject = test_detail::ReflectedObject(42, test_detail::Complex{ "text"s, &i });
	auto reflectionObject = Object(essentials::make_observer(&actualObject));
	
	auto iface = reflectionObject.reflect();

	BOOST_CHECK(iface.hasProperty("i"));

	BOOST_CHECK(iface.hasProperty("complex"));

	//BOOST_CHECK_EQUAL(ponder::runtime::call(clazz.function("foo"), obj), 42);
	//BOOST_CHECK_EQUAL(obj.get("text").to<std::string>(), "To be, or not to be!"s);
}

BOOST_AUTO_TEST_SUITE_END(/* ReflectionInterfaceTestSuite */);

} // anonymous namespace
