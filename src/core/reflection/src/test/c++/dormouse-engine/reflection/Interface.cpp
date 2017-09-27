#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/reflection/Interface.hpp"

using namespace dormouse_engine::reflection;

//namespace test_detail {
//
//struct Complex {
//	std::string s;
//	int i;
//};
//
//struct ReflectedObject {
//
//	ReflectedObject(int i, Complex complex) :
//		i_(i),
//		complex_(complex)
//	{
//	}
//
//	int i() const {
//		return i_;
//	}
//
//	const Complex& complex() const {
//		return complex_;
//	}
//
//private:
//
//	int i_;
//
//	Complex complex_;
//
//	friend void declareReflectedObject();
//
//};
//
//void declareComplex() {
//	ponder::Class::declare<Complex>("test_detail::Complex")
//		.property("s", &Complex::s)
//		.property("i", &Complex::i)
//		;
//}
//
//void declareReflectedObject() {
//	ponder::Class::declare<ReflectedObject>("test_detail::ReflectedObject")
//		.constructor<int, Complex>()
//		.property("i", &ReflectedObject::i).tag(Tag::SHADER_PARAMETER)
//		.property("complex", &ReflectedObject::complex)
//		;
//}
//
//const Interface reflectObject([[maybe_unused]] const ReflectedObject& object) {
//	return Interface(essentials::make_observer(&ponder::classByType<ReflectedObject>()));
//}
//
//} // namespace test_detail

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(ReflectionInterfaceTestSuite);

BOOST_AUTO_TEST_CASE(CanCallMemberFunctions) {
}

BOOST_AUTO_TEST_SUITE_END(/* ReflectionInterfaceTestSuite */);

} // anonymous namespace
