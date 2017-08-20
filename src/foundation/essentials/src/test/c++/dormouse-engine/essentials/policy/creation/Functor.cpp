#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <functional>

#include "dormouse-engine/essentials/policy/creation/Functor.hpp"

namespace {

using namespace dormouse_engine::essentials::policy::creation;

BOOST_AUTO_TEST_SUITE(PolicyTestSuite);
BOOST_AUTO_TEST_SUITE(FunctorTestSuite);

int foo() {
	return -1;
}

int fooWithValue(int value) {
	return value;
}

struct S {
	int operator()() const {
		return -1;
	}
};

BOOST_AUTO_TEST_CASE(IsCreatableWithStdFunctionType) {
	Functor<std::function<int ()>> functor(foo);
	BOOST_CHECK_EQUAL(functor.create(), -1);
}

BOOST_AUTO_TEST_CASE(IsCreatableWithMakeFunctor) {
	auto functor = makeFunctor(foo);
	BOOST_CHECK_EQUAL(functor.create(), -1);
}

BOOST_AUTO_TEST_CASE(CallsFunctionWithParams) {
	auto functor = makeFunctor<int>(fooWithValue);
	BOOST_CHECK_EQUAL(functor.create(42), 42);
}

BOOST_AUTO_TEST_CASE(CallsFunctor) {
	auto functor = makeFunctor(S());
	BOOST_CHECK_EQUAL(functor.create(), -1);
}

BOOST_AUTO_TEST_SUITE_END(/* FunctorTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PolicyTestSuite */);

} // anonymous namespace
