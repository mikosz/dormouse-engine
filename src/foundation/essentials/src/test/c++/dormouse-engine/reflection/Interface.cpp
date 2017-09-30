#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#pragma warning(push, 3)
#	include <ponder/classbuilder.hpp>
#	include <ponder/uses/runtime.hpp>
#pragma warning(pop)

#include "dormouse-engine/reflection/Object.hpp"
#include "dormouse-engine/reflection/Interface.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::reflection;
using namespace std::string_literals;

namespace interface_test_detail {

struct Complex : public ReflectiveObject<Complex> {

	static constexpr auto CLASS_NAME = "interface_test_detail::Complex";

	Complex() = default;

	Complex(std::string s, int i) :
		s(std::move(s)),
		i(i)
	{
	}

	std::string s;
	int i;
};

struct ReflectedObject : public ReflectiveObject<ReflectedObject> {

	static constexpr auto CLASS_NAME = "interface_test_detail::ReflectedObject";

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

} // namespace interface_test_detail

PONDER_AUTO_TYPE(interface_test_detail::Complex, &interface_test_detail::declareComplex);
PONDER_AUTO_TYPE(interface_test_detail::ReflectedObject, &interface_test_detail::declareReflectedObject);

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(ReflectionSuite);
BOOST_AUTO_TEST_SUITE(InterfaceSuite);

BOOST_AUTO_TEST_CASE(HasPropertyTest) {
	const auto object = interface_test_detail::ReflectedObject(42, interface_test_detail::Complex("test"s, 666));
	const auto iface = Object(essentials::make_observer(&object)).iface();

	BOOST_CHECK_EQUAL(iface.name(), interface_test_detail::ReflectedObject::CLASS_NAME);

	BOOST_CHECK(iface.hasProperty("i"));
	BOOST_CHECK(iface.hasProperty("i", Tag::SHADER_PARAMETER));
	BOOST_CHECK(!iface.hasProperty("i", Tag::SHADER_RESOURCE));

	BOOST_CHECK(iface.hasProperty("complex"));
	BOOST_CHECK(!iface.hasProperty("complex", Tag::SHADER_PARAMETER));
}

BOOST_AUTO_TEST_CASE(ConstructibleByName) {
	{
		// ensure Complex Ponder type registered
		const auto complex = interface_test_detail::Complex();
		Object(essentials::make_observer(&complex)).iface();
	}

	const auto iface = Interface(interface_test_detail::Complex::CLASS_NAME);
	BOOST_CHECK_EQUAL(iface.name(), interface_test_detail::Complex::CLASS_NAME);
}

BOOST_AUTO_TEST_SUITE_END(/* InterfaceSuite */);
BOOST_AUTO_TEST_SUITE_END(/* ReflectionSuite */);

} // anonymous namespace
