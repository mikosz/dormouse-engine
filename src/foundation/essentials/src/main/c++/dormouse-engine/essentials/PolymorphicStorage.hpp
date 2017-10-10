#ifndef _DORMOUSEENGINE_ESSENTIALS_POLYMORPHICSTORAGE_HPP_
#define _DORMOUSEENGINE_ESSENTIALS_POLYMORPHICSTORAGE_HPP_

#include <utility>
#include <type_traits>

namespace dormouse_engine::essentials {

class ConceptBase {
public:

	virtual ~ConceptBase() = default;

	virtual void copy(void* where) const = 0;

	virtual void move(void* where) = 0;

};

template <class ConceptType, class ModelType, class StoredType>
class ModelBase : public ConceptType {
public:

	ModelBase(StoredType model) :
		model_(std::move(model))
	{
	}

	void copy(void* where) const override final {
		new(where) ModelType(model_);
	}

	void move(void* where) override final {
		new(where) ModelType(std::move(model_));
	}

protected:

	StoredType model_;

};

template <class ConceptType, template<class> class ModelType, size_t SIZE, size_t ALIGNMENT>
class PolymorphicStorage final {
public:

	template <class T>
	PolymorphicStorage(T model) {
		static_assert(sizeof(ModelType<T>) <= SIZE);
		static_assert(alignof(ModelType<T>) <= ALIGNMENT);

		new(&storage_) ModelType<T>(std::move(model));
	}

	~PolymorphicStorage() noexcept {
		get()->~ConceptType();
	}

	PolymorphicStorage(const PolymorphicStorage& other) {
		other->copy(&storage_);
	}

	PolymorphicStorage(PolymorphicStorage&& other) {
		other->move(&storage_);
	}

	PolymorphicStorage& operator=(const PolymorphicStorage& other) {
		if (this != &other) {
			get()->~ConceptType();
			other->copy(&storage_);
		}

		return *this;
	}

	PolymorphicStorage& operator=(PolymorphicStorage&& other) {
		if (this != &other) {
			get()->~ConceptType();
			other->move(&storage_);
		}

		return *this;
	}

	const ConceptType* get() const noexcept {
		return reinterpret_cast<const ConceptType*>(&storage_);
	}

	ConceptType* get() noexcept {
		return const_cast<ConceptType*>(const_cast<const PolymorphicStorage&>(*this).get());
	}

	const ConceptType* operator->() const {
		return get();
	}

	ConceptType* operator->() {
		return get();
	}

	const ConceptType& operator*() const {
		return *get();
	}

	ConceptType& operator*() {
		return *get();
	}

private:

	std::aligned_storage_t<SIZE, ALIGNMENT> storage_;

};

} // namespace dormouse_engine::essentials

#endif /* _DORMOUSEENGINE_ESSENTIALS_POLYMORPHICSTORAGE_HPP_ */
