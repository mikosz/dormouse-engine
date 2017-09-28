#ifndef _DORMOUSEENGINE_REFLECTION_INTERFACE_HPP_
#define _DORMOUSEENGINE_REFLECTION_INTERFACE_HPP_

#include <ponder/class.hpp>
#include <ponder/pondertype.hpp>

#include "dormouse-engine/enums/enum.hpp"
#include "dormouse-engine/essentials/StringId.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"

namespace dormouse_engine::reflection {

namespace detail { void declareTag(); }

class Object;

DE_ENUM(
	Tag,
	(NONE)
	(SHADER_RESOURCE)
	(SHADER_PARAMETER)
	);

class Interface {
public:

	Interface(essentials::observer_ptr<const ponder::Class> clazz) :
		clazz_(std::move(clazz))
	{
	}

	essentials::StringId name() const;

	bool hasProperty(essentials::StringId name, Tag withTag = Tag::NONE) const;

	template <class T>
	T getProperty(const Object& object, essentials::StringId name, Tag withTag = Tag::NONE) const {
		return getPropertyValue_(object, name, withTag).to<T>();
	}

private:

	essentials::observer_ptr<const ponder::Class> clazz_;

	ponder::Value Interface::getPropertyValue_(const Object& object, essentials::StringId name, Tag withTag) const;

};

} // namespace dormouse_engine::reflection

PONDER_AUTO_TYPE(dormouse_engine::reflection::Tag, &dormouse_engine::reflection::detail::declareTag);

#endif /* _DORMOUSEENGINE_REFLECTION_INTERFACE_HPP_ */
