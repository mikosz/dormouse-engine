#ifndef _DORMOUSEENGINE_REFLECTION_OBJECT_HPP_
#define _DORMOUSEENGINE_REFLECTION_OBJECT_HPP_

#include "dormouse-engine/essentials/StringId.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/essentials/PolymorphicStorage.hpp"
#include "Interface.hpp"

namespace dormouse_engine::reflection {

class Interface;

class Object final {
public:

	template <class T>
	Object(essentials::observer_ptr<T> ptr) :
		storage_(std::move(ptr))
	{
	}

	const Interface reflect() const {
		return storage_->reflect();
	}

private:

	class Concept : public essentials::ConceptBase {
	public:

		virtual const Interface reflect() const = 0;

	};

	template <class T>
	class Model : public essentials::ModelBase<Concept, Model<T>, T> {
	public:

		Model(T ptr) :
			essentials::ModelBase<Concept, Model<T>, T>(std::move(ptr))
		{
		}

		virtual const Interface reflect() const override {
			return reflectObject(*model_);
		}

	};

	static constexpr auto STORAGE_SIZE = 2 * sizeof(void*);
	static constexpr auto STORAGE_ALIGNMENT = alignof(void*);

	essentials::PolymorphicStorage<Concept, Model, STORAGE_SIZE, STORAGE_ALIGNMENT> storage_;

};

class ReflectiveObject;
Interface reflectObject(const ReflectiveObject& object);

class ReflectiveObject {
public:

	ReflectiveObject(essentials::StringId name) :
		name_(name)
	{
	}

private:

	essentials::StringId name_;

	friend Interface reflection::reflectObject(const ReflectiveObject& object);

};

} // namespace dormouse_engine::reflection

#endif /* _DORMOUSEENGINE_REFLECTION_OBJECT_HPP_ */
