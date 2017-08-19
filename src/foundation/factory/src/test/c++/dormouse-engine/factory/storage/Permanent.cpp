#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/factory/storage/Permanent.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::factory;
using namespace dormouse_engine::factory::storage;

namespace {

BOOST_AUTO_TEST_SUITE(FactoryTestSuite);
BOOST_AUTO_TEST_SUITE(TestSuite);
BOOST_AUTO_TEST_SUITE(PermanentTestSuite);

BOOST_AUTO_TEST_CASE(GetReturnsCreatedInstance) {
	using Permanent = Permanent<std::string, std::unique_ptr<int>>;
	Permanent storage;

	const auto creator = []() { return std::make_unique<int>(42); };

	BOOST_CHECK_EQUAL(*storage.get("instance", creator), 42);
}

BOOST_AUTO_TEST_CASE(StoreReplacesExistingEntry) {
	using Permanent = Permanent<std::string, std::unique_ptr<int>>;
	Permanent storage;

	int* instance = new int;
	int* newInstance = new int;

	auto stored = storage.store("instance", std::unique_ptr<int>(instance));
	auto newStored = storage.store("instance", std::unique_ptr<int>(newInstance));

	const auto nullCreator = []() { return std::unique_ptr<int>(); };

	BOOST_CHECK_EQUAL(stored.get(), instance);
	BOOST_CHECK_EQUAL(newStored.get(), newInstance);

	BOOST_CHECK_EQUAL(storage.get("instance", nullCreator).get(), newInstance);
}

BOOST_AUTO_TEST_CASE(ReturnsStoredInstanceWhenExists) {
	using Permanent = Permanent<std::string, std::unique_ptr<int>>;
	Permanent storage;

	int* instance = new int;

	auto stored = storage.store("instance", std::unique_ptr<int>(instance));

	const auto nullCreator = []() { return std::unique_ptr<int>(); };
	
	BOOST_CHECK_EQUAL(stored.get(), instance);
	BOOST_CHECK_EQUAL(storage.get("instance", nullCreator).get(), instance);
}

BOOST_AUTO_TEST_CASE(RetainsInstanceWhenUnused) {
	using Permanent = Permanent<std::string, std::unique_ptr<int>>;
	Permanent storage;

	int* instance = new int;

	storage.store("instance", std::unique_ptr<int>(instance));

	const auto nullCreator = []() { return std::unique_ptr<int>(); };
	
	BOOST_CHECK(storage.get("instance", nullCreator));
	BOOST_CHECK_EQUAL(storage.get("instance", nullCreator).get(), instance);
}

BOOST_AUTO_TEST_CASE(ErasesExistingInstances) {
	using Permanent = Permanent<std::string, std::unique_ptr<int>>;
	Permanent storage;

	int* instance = new int;

	storage.store("instance", std::unique_ptr<int>(instance));
	storage.erase("instance");

	const auto nullCreator = []() { return std::unique_ptr<int>(); };
	
	BOOST_CHECK(!storage.get("instance", nullCreator));
}

BOOST_AUTO_TEST_CASE(EraseIsNoOpOnNoExistingInstances) {
	using Permanent = Permanent<std::string, std::unique_ptr<int>>;
	Permanent storage;

	storage.erase("instance");

	const auto nullCreator = []() { return std::unique_ptr<int>(); };
	
	BOOST_CHECK(!storage.get("instance", nullCreator));
}

BOOST_AUTO_TEST_CASE(ClearErasesAll) {
	using Permanent = Permanent<std::string, std::unique_ptr<int>>;
	Permanent storage;

	storage.store("instance", std::unique_ptr<int>(new int));
	storage.store("another", std::unique_ptr<int>(new int));

	storage.clear();

	const auto nullCreator = []() { return std::unique_ptr<int>(); };

	BOOST_CHECK(!storage.get("instance", nullCreator));
	BOOST_CHECK(!storage.get("another", nullCreator));
}

BOOST_AUTO_TEST_CASE(IsStoredReturnsTrueIfInstancePresent) {
	using Permanent = Permanent<std::string, int>;
	Permanent storage;

	int* instance = new int;

	storage.store("instance", std::unique_ptr<int>(instance));

	BOOST_CHECK(storage.isStored("instance"));
}

BOOST_AUTO_TEST_CASE(IsStoredReturnsFalseIfInstanceNotPresent) {
	using Permanent = Permanent<std::string, int>;
	Permanent storage;

	BOOST_CHECK(!storage.isStored("instance"));
}

BOOST_AUTO_TEST_CASE(IsStoredReturnsTrueIfInstanceRemoved) {
	using Permanent = Permanent<std::string, int>;
	Permanent storage;

	int* instance = new int;

	storage.store("instance", std::unique_ptr<int>(instance));
	storage.erase("instance");

	BOOST_CHECK(!storage.isStored("instance"));
}

BOOST_AUTO_TEST_SUITE_END(/* PermanentTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* StorageTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* FactoryTestSuite */);

} // anonymous namespace
