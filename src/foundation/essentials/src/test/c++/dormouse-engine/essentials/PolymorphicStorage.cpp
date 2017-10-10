#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/essentials/test-utils/ConstructionRegisteringObject.hpp"
#include "dormouse-engine/essentials/PolymorphicStorage.hpp"
#include "dormouse-engine/enums/enum.hpp"

using namespace dormouse_engine::essentials;

namespace /* anonymous */ {

DE_ENUM(
	Version,
	(INT)
	(FLOAT)
	(CONSTRUCTION_REGISTERING_OBJECT)
);

class Concept : public ConceptBase {
public:

	virtual ~Concept() = default;

	virtual Version version() const = 0;

	virtual const void* modelPtr() const = 0;

};

template <class T>
class Model : public ModelBase<Concept, Model<T>, T> {
public:

	Model(T model) :
		ModelBase(std::move(model))
	{
	}

	Version version() const override {
		return ::version(model_);
	}

	const void* modelPtr() const override {
		return &model_;
	}

};

Version version(int) {
	return Version::INT;
}

Version version(float) {
	return Version::FLOAT;
}

Version version(const test_utils::ConstructionRegisteringObject&) {
	return Version::CONSTRUCTION_REGISTERING_OBJECT;
}

using Storage = PolymorphicStorage<Concept, Model, 2 * sizeof(void*), alignof(void*)>;

BOOST_AUTO_TEST_SUITE(PolymorphicStorageTestSuite);

BOOST_AUTO_TEST_CASE(CallsAppropriateConstrutors) {
	auto registry = test_utils::ConstructionRegistry();
	const auto* storageModelPtr = static_cast<const void*>(nullptr);
	const auto* copyModelPtr = static_cast<const void*>(nullptr);
	const auto* moveCopyModelPtr = static_cast<const void*>(nullptr);

	{
		auto storage = Storage(test_utils::ConstructionRegisteringObject(registry));
		storageModelPtr = storage->modelPtr();
		BOOST_CHECK(registry.objects[storageModelPtr].moveConstructed);

		auto copy = storage;
		copyModelPtr = copy->modelPtr();
		BOOST_CHECK(!registry.objects[storageModelPtr].movedFrom);
		BOOST_CHECK(registry.objects[copyModelPtr].copyConstructed || registry.objects[copyModelPtr].moveConstructed);

		auto moveCopy = std::move(copy);
		moveCopyModelPtr = moveCopy->modelPtr();
		BOOST_CHECK(registry.objects[copyModelPtr].movedFrom);
		BOOST_CHECK(registry.objects[moveCopyModelPtr].moveConstructed);
	}

	BOOST_CHECK(registry.objects[storageModelPtr].destructed);
	BOOST_CHECK(registry.objects[copyModelPtr].destructed);
	BOOST_CHECK(registry.objects[moveCopyModelPtr].destructed);
}

BOOST_AUTO_TEST_CASE(HasValueSemantics) {
	auto storage = Storage(42);
	const auto* modelIntPtr = dynamic_cast<const Model<int>*>(storage.get());
	BOOST_CHECK_NE(modelIntPtr, nullptr);
	BOOST_CHECK_EQUAL(storage->version(), Version::INT);

	auto f = Storage(2.0f);
	BOOST_CHECK_EQUAL(f->version(), Version::FLOAT);

	storage = f;

	const auto* modelFloatPtr = dynamic_cast<const Model<float>*>(storage.get());
	BOOST_CHECK_NE(modelFloatPtr, nullptr);
	BOOST_CHECK_EQUAL(storage->version(), Version::FLOAT);
}

BOOST_AUTO_TEST_SUITE_END(/* PolymorphicStorageTestSuite */);

} // anonymous namespace
