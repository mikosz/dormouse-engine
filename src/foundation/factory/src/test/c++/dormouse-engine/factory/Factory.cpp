#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <string>
#include <memory>
#include <functional>

#include <gmock/gmock.h>

#include "dormouse-engine/factory/Factory.hpp"
#include "dormouse-engine/factory/storage.hpp"
#include "dormouse-engine/factory/error-policy.hpp"

#include "dormouse-engine/essentials/test-utils/GMockFixture.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::factory;

using namespace std::string_literals;

namespace /* anonymous */ {

class MockCreator {
public:

	using Instance = std::shared_ptr<int>;

	MOCK_METHOD1(doCreate, std::shared_ptr<int> (const std::string&));

};

class MockParametrisedCreator {
public:

	using Instance = std::shared_ptr<int>;

	MOCK_METHOD2(doCreate, std::shared_ptr<int> (const std::string&, int));

};

class MockStorage {
public:

	using Creator = std::function<std::shared_ptr<int>()>;

	MOCK_CONST_METHOD1(get, int (const std::string&));

	MOCK_METHOD2(store, int (const std::string&, int));

	MOCK_METHOD1(erase, void (const std::string&));

};

template <class, class>
class SingletonMockStorageAdapter {
public:

	using Delegate = testing::StrictMock<MockStorage>;

	using Instance = std::shared_ptr<int>;

	~SingletonMockStorageAdapter() {
		reset();
	}

	static void reset() {
		delegate_.reset();
	}

	static std::shared_ptr<Delegate> delegate() {
		if (!delegate_) {
			delegate_.reset(new Delegate);
		}
		return delegate_;
	}

	Instance get(const std::string& id, MockStorage::Creator creator) {
		auto value = delegate()->get(id);
		if (value) {
			return std::make_unique<int>(value);
		} else {
			return creator();
		}
	}

	Instance store(const std::string& id, std::shared_ptr<int> instance) {
		return std::make_shared<int>(delegate()->store(id, *instance));
	}

	void erase(const std::string& id) {
		delegate()->erase(id);
	}

private:

	static std::shared_ptr<Delegate> delegate_;

};

template<class T1, class T2>
std::shared_ptr<typename SingletonMockStorageAdapter<T1, T2>::Delegate> SingletonMockStorageAdapter<T1, T2>::delegate_;

struct IntConstructedCreator {

	using Instance = int;

	IntConstructedCreator(int value) :
		value(value)
	{
	}

	int value;

};

BOOST_AUTO_TEST_SUITE(FactoryTestSuite);
BOOST_FIXTURE_TEST_SUITE(FactoryTestSuite, essentials::test_utils::GMockFixture);

BOOST_AUTO_TEST_CASE(CallsCreators) {
	Factory<
		std::string,
		MockCreator,
		storage::None
		> f;

	EXPECT_CALL(f, doCreate("1")).WillOnce(testing::Return(std::make_shared<int>(1)));
	EXPECT_CALL(f, doCreate("2")).WillOnce(testing::Return(std::make_shared<int>(2)));

	BOOST_CHECK_EQUAL(*f.create("1"), 1);
	BOOST_CHECK_EQUAL(*f.create("2"), 2);
}

BOOST_AUTO_TEST_CASE(StoresCreatedInstances) {
	using Storage = SingletonMockStorageAdapter<std::string, std::shared_ptr<int>>;

	Storage::reset();

	Factory<
		std::string,
		MockCreator,
		SingletonMockStorageAdapter
		> f;

	{
		testing::InSequence inSequence;
		using testing::_;

		EXPECT_CALL(*Storage::delegate(), get("1"s))
			.WillOnce(testing::Return(0))
			.WillOnce(testing::Return(1));

		EXPECT_CALL(*Storage::delegate(), get("2"s))
			.WillOnce(testing::Return(0))
			.WillOnce(testing::Return(2));
	}

	EXPECT_CALL(f, doCreate("1")).WillOnce(testing::Return(std::make_shared<int>(1)));
	EXPECT_CALL(f, doCreate("2")).WillOnce(testing::Return(std::make_shared<int>(2)));

	auto one1 = f.create("1");
	auto one2 = f.create("1");
	auto two1 = f.create("2");
	auto two2 = f.create("2");

	BOOST_REQUIRE(one1);
	BOOST_REQUIRE(one2);
	BOOST_REQUIRE(two1);
	BOOST_REQUIRE(two2);

	BOOST_CHECK_EQUAL(*one1, 1);
	BOOST_CHECK_EQUAL(*one2, 1);
	BOOST_CHECK_EQUAL(*two1, 2);
	BOOST_CHECK_EQUAL(*two2, 2);
}

BOOST_AUTO_TEST_CASE(PassesArgumentsToCreatorConstructor) {
	Factory<
		int,
		IntConstructedCreator,
		storage::None
		> f(42);

	BOOST_CHECK_EQUAL(f.value, 42);
}

BOOST_AUTO_TEST_CASE(PassesArgumentsToCreatorCreator) {
	Factory<
		std::string,
		MockParametrisedCreator,
		storage::None
		> f;

	EXPECT_CALL(f, doCreate("1", 42)).WillOnce(testing::Return(std::make_shared<int>(42)));
	EXPECT_CALL(f, doCreate("2", 123)).WillOnce(testing::Return(std::make_shared<int>(123)));

	BOOST_CHECK_EQUAL(*f.create("1", 42), 42);
	BOOST_CHECK_EQUAL(*f.create("2", 123), 123);
}

BOOST_AUTO_TEST_SUITE_END(/* FactoryTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* FactoryTestSuite */);

} // anonymous namespace
