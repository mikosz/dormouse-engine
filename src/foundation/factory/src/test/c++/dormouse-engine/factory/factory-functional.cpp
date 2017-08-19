#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <functional>

#include "dormouse-engine/factory.hpp"

#include "dormouse-engine/essentials/policy/creation/New.hpp"
#include "dormouse-engine/essentials/policy/creation/Functor.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::factory;

namespace /* anonymous */ {

class AbstractClass {
public:

	virtual ~AbstractClass() {
	}

	virtual const std::string& id() const = 0;

};

class ConcreteClass1 : public AbstractClass {
public:

	static const std::string ID;

	const std::string& id() const override {
		return ID;
	}

};

const std::string ConcreteClass1::ID("ConcreteClass1");

class ConcreteClass2 : public AbstractClass {
public:

	static const std::string ID;

	const std::string& id() const override {
		return ID;
	}

};

int return2() {
	return 2;
}

struct Return3 {

	int operator()() {
		return 3;
	}

};

const std::string ConcreteClass2::ID("ConcreteClass2");

BOOST_AUTO_TEST_SUITE(DesignPatternTestSuite);
BOOST_AUTO_TEST_SUITE(FactoryFunctionalTestSuite);

BOOST_AUTO_TEST_CASE(RegisteredTypesCreatingFactory) {
	using Factory = Factory<
		std::string,
		CreatorRegistry<std::string, policy::creation::New<AbstractClass>, error_policy::ExceptionThrowing>,
		storage::None
		>;

	Factory f;

	f.registerCreator(ConcreteClass1::ID, policy::creation::New<AbstractClass>::makeCreator<ConcreteClass1>());
	f.registerCreator(ConcreteClass2::ID, policy::creation::New<AbstractClass>::makeCreator<ConcreteClass2>());

	BOOST_CHECK_THROW(
			f.registerCreator(ConcreteClass1::ID, policy::creation::New<AbstractClass>::makeCreator<ConcreteClass1>()),
			error_policy::CreatorAlreadyRegistered<std::string>);
	BOOST_CHECK_THROW(
			f.registerCreator(ConcreteClass2::ID, policy::creation::New<AbstractClass>::makeCreator<ConcreteClass2>()),
			error_policy::CreatorAlreadyRegistered<std::string>);

	auto instance1 = f.create(ConcreteClass1::ID);
	auto instance2 = f.create(ConcreteClass2::ID);

	BOOST_CHECK_EQUAL(instance1->id(), ConcreteClass1::ID);
	BOOST_CHECK_EQUAL(instance2->id(), ConcreteClass2::ID);

	f.unregisterCreator(ConcreteClass1::ID);
	f.unregisterCreator(ConcreteClass2::ID);

	BOOST_CHECK_THROW(f.create(ConcreteClass1::ID), error_policy::NoSuchType<std::string>);
	BOOST_CHECK_THROW(f.create(ConcreteClass2::ID), error_policy::NoSuchType<std::string>);
}

BOOST_AUTO_TEST_CASE(RegisteredFunctorsCreatingFactory) {
	using FunctorType = std::function<int()>;
	using Factory = Factory<
		int,
		CreatorRegistry<int, policy::creation::Functor<FunctorType>, error_policy::ExceptionThrowing>,
		storage::None
		>;

	Factory f;

	f.registerCreator(1, []() { return 1; });
	f.registerCreator(2, &return2);
	f.registerCreator(3, Return3());

	BOOST_CHECK_THROW(
			f.registerCreator(1, []() { return 1; }),
			error_policy::CreatorAlreadyRegistered<int>
			);

	BOOST_CHECK_EQUAL(f.create(1), 1);
	BOOST_CHECK_EQUAL(f.create(2), 2);
	BOOST_CHECK_EQUAL(f.create(3), 3);

	f.unregisterCreator(1);
	f.unregisterCreator(2);
	f.unregisterCreator(3);

	BOOST_CHECK_THROW(f.create(1), error_policy::NoSuchType<int>);
	BOOST_CHECK_THROW(f.create(2), error_policy::NoSuchType<int>);
	BOOST_CHECK_THROW(f.create(3), error_policy::NoSuchType<int>);
}

BOOST_AUTO_TEST_CASE(CachingFactory) {
	using FunctorType = std::function<std::unique_ptr<int>()>;
	using Factory = Factory<
		int,
		CreatorRegistry<int, policy::creation::Functor<FunctorType>, error_policy::ExceptionThrowing>,
		storage::Permanent
		>;

	Factory f;

	f.registerCreator(1, policy::creation::Functor<FunctorType>([]() { return std::unique_ptr<int>(new int(1)); }));
	f.registerCreator(2, policy::creation::Functor<FunctorType>([]() { return std::unique_ptr<int>(new int(2)); }));

	auto one = f.create(1);
	auto two = f.create(2);

	auto oneCopy = f.create(1);
	auto twoCopy = f.create(2);

	BOOST_CHECK_EQUAL(one.get(), oneCopy.get());
	BOOST_CHECK_EQUAL(two.get(), twoCopy.get());
	BOOST_CHECK(one.get() != two.get());
}

BOOST_AUTO_TEST_SUITE_END(/* FactoryFunctionalTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* DesignPatternTestSuite */);

} // anonymous namespace
