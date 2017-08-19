#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <string>
#include <memory>

#include <boost/thread/mutex.hpp>

#include <gmock/gmock.h>

#include "dormouse-engine/essentials/policy/creation/New.hpp"

#include "dormouse-engine/factory/CreatorRegistry.hpp"

#include "dormouse-engine/essentials/test-utils/GMockFixture.hpp"

namespace {

using namespace dormouse_engine;
using namespace dormouse_engine::factory;

class MockErrorPolicy {
public:

	MOCK_METHOD1(alreadyRegistered, void (const std::string&));

	MOCK_METHOD1(noSuchType, void (const std::string&));

};

template <class>
class StaticFunctionMockErrorPolicyAdapter {
public:

	using Delegate = testing::StrictMock<MockErrorPolicy>;

	static void reset() {
		delegate_.reset();
	}

	static std::shared_ptr<Delegate> delegate() {
		if (!delegate_) {
			delegate_.reset(new Delegate);
		}
		return delegate_;
	}

	static void alreadyRegistered(const std::string& id) {
		delegate_->alreadyRegistered(id);
	}

	static void noSuchType(const std::string& id) {
		delegate_->noSuchType(id);
	}

private:

	static std::shared_ptr<Delegate> delegate_;

};

template <class T>
std::shared_ptr<typename StaticFunctionMockErrorPolicyAdapter<T>::Delegate> StaticFunctionMockErrorPolicyAdapter<T>::delegate_;

class MockCreationPolicy {
public:

	using Instance = std::shared_ptr<int>;

	MOCK_METHOD0(create, Instance ());

	MOCK_METHOD1(create, Instance (int));

};

class CopyableMockCreationPolicyAdapter {
public:

	using Instance = std::shared_ptr<int>;

	using Delegate = testing::StrictMock<MockCreationPolicy>;

	static std::shared_ptr<Delegate> delegate() {
		if (!delegate_) {
			delegate_ = std::make_shared<Delegate>();
		}
		return delegate_;
	}

	Instance create() {
		return delegate_->create();
	}

	Instance create(int i) {
		return delegate_->create(i);
	}

	static void reset() {
		delegate_.reset();
	}

private:

	static std::shared_ptr<Delegate> delegate_;

};

std::shared_ptr<CopyableMockCreationPolicyAdapter::Delegate> CopyableMockCreationPolicyAdapter::delegate_;

template <class CreatorRegistryType>
class DoCreateExposer : public CreatorRegistryType {
public:

	using CreatorRegistry::doCreate;

};

BOOST_AUTO_TEST_SUITE(FactoryTestSuite);
BOOST_FIXTURE_TEST_SUITE(CreatorRegistryTestSuite, essentials::test_utils::GMockFixture);

BOOST_AUTO_TEST_CASE(ExecutesRegisteredCreators) {
	using ErrorPolicy = StaticFunctionMockErrorPolicyAdapter<std::string>;

	CopyableMockCreationPolicyAdapter::reset();
	ErrorPolicy::reset();

	DoCreateExposer<
		CreatorRegistry<
			std::string,
			CopyableMockCreationPolicyAdapter,
			StaticFunctionMockErrorPolicyAdapter
			>
		> cr;

	EXPECT_CALL(*CopyableMockCreationPolicyAdapter::delegate(), create())
		.WillOnce(testing::Return(std::make_shared<int>(1)))
		.WillOnce(testing::Return(std::make_shared<int>(2)))
		;

	cr.registerCreator("1", CopyableMockCreationPolicyAdapter());
	cr.registerCreator("2", CopyableMockCreationPolicyAdapter());

	auto one = cr.doCreate("1");
	auto two = cr.doCreate("2");

	BOOST_CHECK_EQUAL(*one, 1);
	BOOST_CHECK_EQUAL(*two, 2);

	ErrorPolicy::reset();
	CopyableMockCreationPolicyAdapter::reset();
}

BOOST_AUTO_TEST_CASE(PassesArgumentsToCreators) {
	using ErrorPolicy = StaticFunctionMockErrorPolicyAdapter<std::string>;

	CopyableMockCreationPolicyAdapter::reset();
	ErrorPolicy::reset();

	DoCreateExposer<
		CreatorRegistry<
			std::string,
			CopyableMockCreationPolicyAdapter,
			StaticFunctionMockErrorPolicyAdapter
			>
		> cr;

	EXPECT_CALL(*CopyableMockCreationPolicyAdapter::delegate(), create(1))
		.WillOnce(testing::Return(std::make_shared<int>(1)))
		;

	cr.registerCreator("1", CopyableMockCreationPolicyAdapter());

	auto one = cr.doCreate("1", 1);

	BOOST_CHECK_EQUAL(*one, 1);

	ErrorPolicy::reset();
	CopyableMockCreationPolicyAdapter::reset();
}

BOOST_AUTO_TEST_CASE(ReturnsWhetherCreatorRegistered) {
	using ErrorPolicy = StaticFunctionMockErrorPolicyAdapter<std::string>;

	CopyableMockCreationPolicyAdapter::reset();
	ErrorPolicy::reset();

	CreatorRegistry<
		std::string,
		CopyableMockCreationPolicyAdapter,
		StaticFunctionMockErrorPolicyAdapter
		> cr;

	cr.registerCreator("1", CopyableMockCreationPolicyAdapter());

	BOOST_CHECK(cr.isCreatorRegistered("1"));
	BOOST_CHECK(!cr.isCreatorRegistered("2"));

	ErrorPolicy::reset();
	CopyableMockCreationPolicyAdapter::reset();
}

BOOST_AUTO_TEST_CASE(CallsNoSuchTypeIfCreatingAndCreatorNotRegistered) {
	using ErrorPolicy = StaticFunctionMockErrorPolicyAdapter<std::string>;

	ErrorPolicy::reset();

	DoCreateExposer<
		CreatorRegistry<
			std::string,
			MockCreationPolicy,
			StaticFunctionMockErrorPolicyAdapter
			>
		> cr;

	EXPECT_CALL(*ErrorPolicy::delegate(), noSuchType(std::string("1")));

	cr.doCreate("1");

	ErrorPolicy::reset();
}

BOOST_AUTO_TEST_CASE(CallsNoSuchTypeIfUnregisteringAndCreatorNotRegistered) {
	using ErrorPolicy = StaticFunctionMockErrorPolicyAdapter<std::string>;

	ErrorPolicy::reset();

	DoCreateExposer<
		CreatorRegistry<
			std::string,
			MockCreationPolicy,
			StaticFunctionMockErrorPolicyAdapter
			>
		> cr;

	EXPECT_CALL(*ErrorPolicy::delegate(), noSuchType(std::string("1")));

	cr.unregisterCreator("1");

	ErrorPolicy::reset();
}

BOOST_AUTO_TEST_CASE(CallsCreatorAlreadyRegisteredIfRegisteringAndCreatorRegistered) {
	using ErrorPolicy = StaticFunctionMockErrorPolicyAdapter<std::string>;

	ErrorPolicy::reset();

	DoCreateExposer<
		CreatorRegistry<
			std::string,
			CopyableMockCreationPolicyAdapter,
			StaticFunctionMockErrorPolicyAdapter
			>
		> cr;

	EXPECT_CALL(*ErrorPolicy::delegate(), alreadyRegistered(std::string("1")));

	CopyableMockCreationPolicyAdapter creator;

	cr.registerCreator("1", creator);
	cr.registerCreator("1", creator);

	ErrorPolicy::reset();
}

BOOST_AUTO_TEST_SUITE_END(/* CreatorRegistryTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* FactoryTestSuite */);

} // anonymous namespace
