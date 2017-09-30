#ifndef _DORMOUSEENGINE_REFLECTION_OBJECT_HPP_
#define _DORMOUSEENGINE_REFLECTION_OBJECT_HPP_

#include <type_traits>

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

	template <class T>
	T property(essentials::StringId name) const {
		auto v = storage_->property(name);
		if constexpr (std::is_reference_v<T>) {
			return v.cref<ponder::UserObject>().cref<std::remove_reference_t<T>>();
		} else {
			return v.to<T>();
		}
	}

	template <class T>
	T property(essentials::StringId name) {
		auto v = storage_->property(name);
		if constexpr (std::is_reference_v<T>) {
			return v.ref<ponder::UserObject>().ref<std::remove_reference_t<T>>();
		} else {
			return v.to<T>();
		}
	}

private:

	class Concept : public essentials::ConceptBase {
	public:

		virtual const Interface iface() const = 0;

		virtual ponder::Value property(essentials::StringId name) const = 0;

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

		virtual ponder::Value property(essentials::StringId name) const {
			return iface().property(ponder::UserObject::makeRef(*model_), name);
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
