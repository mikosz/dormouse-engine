#ifndef _DORMOUSEENGINE_REFLECTION_INTERFACE_HPP_
#define _DORMOUSEENGINE_REFLECTION_INTERFACE_HPP_

#include <ponder/class.hpp>

#include "dormouse-engine/essentials/StringId.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"

namespace dormouse_engine::reflection {

class Object;

class Interface {
public:

	Interface(essentials::observer_ptr<const ponder::Class> clazz) :
		clazz_(std::move(clazz))
	{
	}

	bool hasProperty(essentials::StringId name) const;

private:

	essentials::observer_ptr<const ponder::Class> clazz_;

};

} // namespace dormouse_engine::reflection

#endif /* _DORMOUSEENGINE_REFLECTION_INTERFACE_HPP_ */
