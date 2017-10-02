#ifndef _DORMOUSEENGINE_REFLECTION_OBJECT_HPP_
#define _DORMOUSEENGINE_REFLECTION_OBJECT_HPP_

#include <type_traits>

#include <ponder/class.hpp>
#include <ponder/pondertype.hpp>

#include "dormouse-engine/enums/enum.hpp"
#include "dormouse-engine/essentials/StringId.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/essentials/PolymorphicStorage.hpp"

namespace dormouse_engine::reflection {

DE_ENUM(
	Tag,
	(NONE)
	(SHADER_RESOURCE)
	(SHADER_PARAMETER)
	);

namespace detail { void declareTag(); }

class Object final {
public:

	template <class T>
	Object(essentials::observer_ptr<T> ptr) :
		storage_(std::move(ptr))
	{
	}

	const ponder::Class& metaclass() const {
		return storage_->metaclass();
	}

private:

	class Concept : public essentials::ConceptBase {
	public:

		virtual ponder::Class& metaclass() const = 0;

	};

	template <class T>
	class Model : public essentials::ModelBase<Concept, Model<T>, T> {
	public:

		Model(T ptr) :
			essentials::ModelBase<Concept, Model<T>, T>(std::move(ptr))
		{
		}

		virtual ponder::Class& metaclass() const override {
			return ponder::classByObject(*model_);
		}

	};

	static constexpr auto STORAGE_SIZE = 2 * sizeof(void*);
	static constexpr auto STORAGE_ALIGNMENT = alignof(void*);

	essentials::PolymorphicStorage<Concept, Model, STORAGE_SIZE, STORAGE_ALIGNMENT> storage_;

};

class ReflectiveObject {
	PONDER_POLYMORPHIC();
};

namespace detail { void declareReflectiveObject(); }

} // namespace dormouse_engine::reflection

PONDER_AUTO_TYPE(dormouse_engine::reflection::Tag, &dormouse_engine::reflection::detail::declareTag);
PONDER_AUTO_TYPE(
	dormouse_engine::reflection::ReflectiveObject, &dormouse_engine::reflection::detail::declareReflectiveObject);

#endif /* _DORMOUSEENGINE_REFLECTION_OBJECT_HPP_ */
