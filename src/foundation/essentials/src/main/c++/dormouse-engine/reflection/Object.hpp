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

	const Interface iface() const {
		return storage_->iface();
	}



private:

	class Concept : public essentials::ConceptBase {
	public:

		virtual const Interface iface() const = 0;

	};

	template <class T>
	class Model : public essentials::ModelBase<Concept, Model<T>, T> {
	public:

		Model(T ptr) :
			essentials::ModelBase<Concept, Model<T>, T>(std::move(ptr))
		{
		}

		virtual const Interface iface() const override {
			return objectInterface(*model_);
		}

	};

	static constexpr auto STORAGE_SIZE = 2 * sizeof(void*);
	static constexpr auto STORAGE_ALIGNMENT = alignof(void*);

	essentials::PolymorphicStorage<Concept, Model, STORAGE_SIZE, STORAGE_ALIGNMENT> storage_;

};

template <class ObjectType>
class ReflectiveObject {
};

template <class ObjectType>
Interface objectInterface(const ReflectiveObject<ObjectType>& object) {
	const auto& clazz = ponder::classByObject(static_cast<const ObjectType&>(object));
	return essentials::make_observer(&clazz);
}

} // namespace dormouse_engine::reflection

#endif /* _DORMOUSEENGINE_REFLECTION_OBJECT_HPP_ */
