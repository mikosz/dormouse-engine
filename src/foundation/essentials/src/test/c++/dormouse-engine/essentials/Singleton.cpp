#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <memory>

#include "dormouse-engine/essentials/Singleton.hpp"

namespace {

using namespace dormouse_engine;
using namespace dormouse_engine::essentials;

BOOST_AUTO_TEST_SUITE(SingletonTestSuite);

class SingletonClass : public Singleton<SingletonClass> {
};

BOOST_AUTO_TEST_CASE(CreatesSingletonInstance) {
	BOOST_CHECK_EQUAL(SingletonClass::instance().get(), SingletonClass::instance().get());
}

class ReplacedSingletonClass : public Singleton<ReplacedSingletonClass> {
};

BOOST_AUTO_TEST_CASE(ReplacesExistingInstance) {
	ReplacedSingletonClass* constructed = new ReplacedSingletonClass;
	ReplacedSingletonClass* instance = ReplacedSingletonClass::instance().get();

	BOOST_CHECK_NE(constructed, instance);

	ReplacedSingletonClass::setInstance(std::unique_ptr<ReplacedSingletonClass>(constructed));

	BOOST_CHECK_EQUAL(constructed, ReplacedSingletonClass::instance().get());
	BOOST_CHECK_NE(instance, ReplacedSingletonClass::instance().get());
}

class ProvidedSingletonClass : public Singleton<ProvidedSingletonClass> {
};

BOOST_AUTO_TEST_CASE(UsesProvidedInstance) {
	ProvidedSingletonClass* constructed = new ProvidedSingletonClass;
	ProvidedSingletonClass::setInstance(std::unique_ptr<ProvidedSingletonClass>(constructed));

	BOOST_CHECK_EQUAL(constructed, ProvidedSingletonClass::instance().get());
}

BOOST_AUTO_TEST_SUITE_END(/* SingletonTestSuite */);

} // anonymous namespace
