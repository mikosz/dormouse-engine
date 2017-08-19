#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/factory/storage/Volatile.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::factory;
using namespace dormouse_engine::factory::storage;

namespace {

BOOST_AUTO_TEST_SUITE(FactoryTestSuite);
BOOST_AUTO_TEST_SUITE(StorageTestSuite);
BOOST_AUTO_TEST_SUITE(VolatileTestSuite);

BOOST_AUTO_TEST_CASE(GetReturnsCreatedInstance) {
	using Volatile = Volatile<std::string, std::unique_ptr<int>>;
	Volatile storage;

	const auto creator = []() { return std::make_unique<int>(42); };

	BOOST_CHECK_EQUAL(*storage.get("instance", creator), 42);
}

BOOST_AUTO_TEST_CASE(StoreReplacesExistingEntry) {
	using Volatile = Volatile<std::string, std::unique_ptr<int>>;
	Volatile storage;

	const auto instance = std::make_shared<int>();
	const auto newInstance = std::make_shared<int>();

	auto stored = storage.store("instance", instance).lock();
	auto newStored = storage.store("instance", newInstance).lock();

	const auto nullCreator = []() { return std::unique_ptr<int>(); };

	BOOST_CHECK_EQUAL(stored.get(), instance.get());
	BOOST_CHECK_EQUAL(newStored.get(), newInstance.get());
	BOOST_CHECK_EQUAL(storage.get("instance", nullCreator).get(), newInstance.get());
}

BOOST_AUTO_TEST_CASE(ReturnsStoredInstanceWhenExists) {
	using Volatile = Volatile<std::string, std::unique_ptr<int>>;
	Volatile storage;

	const auto instance = std::make_shared<int>();

	auto stored = storage.store("instance", instance).lock();

	const auto nullCreator = []() { return std::unique_ptr<int>(); };
	
	BOOST_CHECK_EQUAL(stored.get(), instance.get());
	BOOST_CHECK_EQUAL(storage.get("instance", nullCreator).get(), instance.get());
}

BOOST_AUTO_TEST_CASE(InvalidatesInstanceWhenUnused) {
	using Volatile = Volatile<std::string, std::unique_ptr<int>>;
	Volatile storage;

	auto instance = std::make_shared<int>();

	auto stored = storage.store("instance", instance);
	instance.reset();

	const auto nullCreator = []() { return std::unique_ptr<int>(); };
	
	BOOST_CHECK(stored.expired());
	BOOST_CHECK(!storage.get("instance", nullCreator));
}

BOOST_AUTO_TEST_CASE(ErasesExistingInstances) {
	using Volatile = Volatile<std::string, std::unique_ptr<int>>;
	Volatile storage;

	auto instance = std::make_shared<int>();

	auto stored = storage.store("instance", instance);

	storage.erase("instance");

	BOOST_CHECK(!storage.isStored("instance"));
}

BOOST_AUTO_TEST_CASE(EraseIsNoOpOnNoExistingInstances) {
	using Volatile = Volatile<std::string, std::unique_ptr<int>>;
	Volatile storage;

	storage.erase("instance");

	BOOST_CHECK(!storage.isStored("instance"));
}

BOOST_AUTO_TEST_CASE(ClearErasesAll) {
	using Volatile = Volatile<std::string, std::unique_ptr<int>>;
	Volatile storage;

	auto instance = std::make_shared<int>();
	auto another = std::make_shared<int>();

	storage.store("instance", instance);
	storage.store("another", another);

	storage.clear();

	BOOST_CHECK(!storage.isStored("instance"));
	BOOST_CHECK(!storage.isStored("another"));
}

BOOST_AUTO_TEST_CASE(IsStoredReturnsTrueIfInstancePresent) {
	using Volatile = Volatile<std::string, std::unique_ptr<int>>;
	Volatile storage;

	auto instance = std::make_shared<int>();

	auto stored = storage.store("instance", instance);

	BOOST_CHECK(storage.isStored("instance"));
}

BOOST_AUTO_TEST_CASE(IsStoredReturnsFalseIfInstanceNotPresent) {
	using Volatile = Volatile<std::string, int>;
	Volatile storage;

	BOOST_CHECK(!storage.isStored("instance"));
}

BOOST_AUTO_TEST_CASE(IsStoredReturnsFalseIfInstanceRemoved) {
	using Volatile = Volatile<std::string, int>;
	Volatile storage;

	auto instance = std::make_shared<int>();

	auto stored = storage.store("instance", instance);
	storage.erase("instance");

	BOOST_CHECK(!storage.isStored("instance"));
}

BOOST_AUTO_TEST_CASE(IsStoredReturnsFalseIfInstanceInvalid) {
	using Volatile = Volatile<std::string, std::unique_ptr<int>>;
	Volatile storage;

	auto instance = std::make_shared<int>();

	auto stored = storage.store("instance", instance);
	instance.reset();

	BOOST_CHECK(!storage.isStored("instance"));
}

BOOST_AUTO_TEST_SUITE_END(/* VolatileTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* StorageTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* FactoryTestSuite */);

} // anonymous namespace
