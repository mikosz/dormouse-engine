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
	ClassTag,
	(NONE)
	(SERIALISABLE)
	);

namespace detail { void declareClassTag(); }

DE_ENUM(
	PropertyTag,
	(NONE)
	(SHADER_RESOURCE)
	(SHADER_PARAMETER)
	);

namespace detail { void declarePropertyTag(); }

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

	ponder::UserObject metaobject() {
		return storage_->metaobject();
	}

private:

	class Concept : public essentials::ConceptBase {
	public:

		virtual const ponder::Class& metaclass() const = 0;

		virtual ponder::UserObject metaobject() = 0;

	};

	template <class T>
	class Model : public essentials::ModelBase<Concept, Model<T>, T> {
	public:

		Model(T ptr) :
			essentials::ModelBase<Concept, Model<T>, T>(std::move(ptr))
		{
		}

		const ponder::Class& metaclass() const override {
			return ponder::classByObject(*model_);
		}

		ponder::UserObject metaobject() override {
			return ponder::UserObject::makeRef(*model_);
		}

	};

	static constexpr auto STORAGE_SIZE = 2 * sizeof(void*);
	static constexpr auto STORAGE_ALIGNMENT = alignof(void*);

	essentials::PolymorphicStorage<Concept, Model, STORAGE_SIZE, STORAGE_ALIGNMENT> storage_;

};

} // namespace dormouse_engine::reflection

PONDER_AUTO_TYPE(
	dormouse_engine::reflection::ClassTag, &dormouse_engine::reflection::detail::declareClassTag);
PONDER_AUTO_TYPE(
	dormouse_engine::reflection::PropertyTag, &dormouse_engine::reflection::detail::declarePropertyTag);

#endif /* _DORMOUSEENGINE_REFLECTION_OBJECT_HPP_ */
