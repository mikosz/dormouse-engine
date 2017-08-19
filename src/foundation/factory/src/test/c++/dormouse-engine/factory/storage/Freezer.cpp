#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/factory/storage/Freezer.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::factory;
using namespace dormouse_engine::factory::storage;

namespace {

BOOST_AUTO_TEST_SUITE(FactoryTestSuite);
BOOST_AUTO_TEST_SUITE(StorageTestSuite);
BOOST_AUTO_TEST_SUITE(FreezerTestSuite);

BOOST_AUTO_TEST_CASE(StoreReplacesExistingEntry) {
	using Freezer = Freezer<std::string, int>;
	Freezer storage;

	int* instance = new int;
	int* newInstance = new int;

	auto stored = storage.store("instance", std::unique_ptr<int>(instance));
	auto newStored = storage.store("instance", std::unique_ptr<int>(newInstance));

	BOOST_CHECK_EQUAL(stored.get(), instance);
	BOOST_CHECK_EQUAL(newStored.get(), newInstance);
	BOOST_CHECK_EQUAL(storage.get("instance").get(), newInstance);
}

BOOST_AUTO_TEST_CASE(ReturnsStoredInstanceWhenExists) {
	using Freezer = Freezer<std::string, int>;
	Freezer storage;

	int* instance = new int;

	auto stored = storage.store("instance", std::unique_ptr<int>(instance));

	BOOST_CHECK_EQUAL(stored.get(), instance);
	BOOST_CHECK_EQUAL(storage.get("instance").get(), instance);
}

BOOST_AUTO_TEST_CASE(RetainsInstanceWhenUnused) {
	using Freezer = Freezer<std::string, int>;
	Freezer storage;

	int* instance = new int;

	storage.store("instance", std::unique_ptr<int>(instance));

	BOOST_CHECK(storage.get("instance"));
	BOOST_CHECK_EQUAL(storage.get("instance").get(), instance);
}

BOOST_AUTO_TEST_CASE(ErasesExistingInstances) {
	using Freezer = Freezer<std::string, int>;
	Freezer storage;

	int* instance = new int;

	storage.store("instance", std::unique_ptr<int>(instance));
	storage.erase("instance");

	BOOST_CHECK(!storage.get("instance"));
}

BOOST_AUTO_TEST_CASE(EraseIsNoOpOnNoExistingInstances) {
	using Freezer = Freezer<std::string, int>;
	Freezer storage;

	storage.erase("instance");

	BOOST_CHECK(!storage.get("instance"));
}

BOOST_AUTO_TEST_CASE(ClearErasesAll) {
	using Freezer = Freezer<std::string, int>;
	Freezer storage;

	storage.store("instance", std::unique_ptr<int>(new int));
	storage.store("another", std::unique_ptr<int>(new int));

	storage.clear();

	BOOST_CHECK(!storage.get("instance"));
	BOOST_CHECK(!storage.get("another"));
}

BOOST_AUTO_TEST_CASE(IsStoredReturnsTrueIfInstancePresent) {
	using Freezer = Freezer<std::string, int>;
	Freezer storage;

	int* instance = new int;

	storage.store("instance", std::unique_ptr<int>(instance));

	BOOST_CHECK(storage.isStored("instance"));
}

BOOST_AUTO_TEST_CASE(IsStoredReturnsFalseIfInstanceNotPresent) {
	using Freezer = Freezer<std::string, int>;
	Freezer storage;

	BOOST_CHECK(!storage.isStored("instance"));
}

BOOST_AUTO_TEST_CASE(IsStoredReturnsTrueIfInstanceRemoved) {
	using Freezer = Freezer<std::string, int>;
	Freezer storage;

	int* instance = new int;

	storage.store("instance", std::unique_ptr<int>(instance));
	storage.erase("instance");

	BOOST_CHECK(!storage.isStored("instance"));
}

BOOST_AUTO_TEST_CASE(RemovesLRUWhenLimitExceeded) {
	using Freezer = Freezer<int, int>;

	Freezer freezer;
	freezer.setMaxFreezerSize(3);
	freezer.setSizeFunc([](const auto&) { return 1; });

	for (auto i = 0; i < 3; ++i) {
		freezer.store(i, std::make_unique<int>(i));
	}

	for (auto i = 0; i < 3; ++i) {
		BOOST_CHECK(freezer.isStored(i));
	}

	freezer.store(3, std::make_unique<int>(3));

	BOOST_CHECK(!freezer.isStored(0));
	for (auto i = 1; i < 4; ++i) {
		BOOST_CHECK(freezer.isStored(i));
	}

	freezer.get(1);
	freezer.store(4, std::make_unique<int>(4));

	BOOST_CHECK(freezer.isStored(1));
	BOOST_CHECK(!freezer.isStored(2));
	for (auto i = 3; i < 5; ++i) {
		BOOST_CHECK(freezer.isStored(i));
	}

	freezer.erase(1);
	freezer.store(5, std::make_unique<int>(5));
	for (auto i = 3; i < 6; ++i) {
		BOOST_CHECK(freezer.isStored(i));
	}
}

BOOST_AUTO_TEST_CASE(CleansUpWhenSizeFunctionChanges) {
	using Freezer = Freezer<int, int>;

	Freezer freezer;
	freezer.setMaxFreezerSize(3);

	for (auto i = 0u; i < 10u; ++i) {
		freezer.store(i, std::make_unique<int>(i));
	}

	for (auto i = 0u; i < 10u; ++i) {
		BOOST_CHECK(freezer.isStored(i));
	}

	freezer.setSizeFunc([](const int&) { return 1; });

	for (auto i = 0u; i < 7u; ++i) {
		BOOST_CHECK(!freezer.isStored(i));
	}

	for (auto i = 7u; i < 10u; ++i) {
		BOOST_CHECK(freezer.isStored(i));
	}
}

BOOST_AUTO_TEST_CASE(CleansUpWhenLimitChanges) {
	using Freezer = Freezer<int, int>;

	Freezer freezer;
	freezer.setSizeFunc([](const int&) { return 1; });

	for (auto i = 0u; i < 10u; ++i) {
		freezer.store(i, std::make_unique<int>(i));
	}

	for (auto i = 0u; i < 10u; ++i) {
		BOOST_CHECK(freezer.isStored(i));
	}

	freezer.setMaxFreezerSize(3);

	for (auto i = 0u; i < 7u; ++i) {
		BOOST_CHECK(!freezer.isStored(i));
	}

	for (auto i = 7u; i < 10u; ++i) {
		BOOST_CHECK(freezer.isStored(i));
	}
}

BOOST_AUTO_TEST_SUITE_END(/* FreezerTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* StorageTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* FactoryTestSuite */);

} // anonymous namespace
