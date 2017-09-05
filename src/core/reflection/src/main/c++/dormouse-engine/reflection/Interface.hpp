#ifndef _DORMOUSEENGINE_REFLECTION_INTERFACE_HPP_
#define _DORMOUSEENGINE_REFLECTION_INTERFACE_HPP_

#include <unordered_map>

#include "dormouse-engine/exceptions/RuntimeError.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/essentials/StringId.hpp"

namespace dormouse_engine::reflection {

class PropertyNotBound : public exceptions::RuntimeError {
public:

	PropertyNotBound(essentials::StringId id) :
		exceptions::RuntimeError("Required property \"" + id.string() + "\" is not bound")
	{
	}

};

class Interface {
public:

	void addChild(essentials::StringId id, essentials::observer_ptr<Interface> child) {
		children_[std::move(id)] = std::move(child);
	}

private:

	using Children = std::unordered_map<essentials::StringId, essentials::observer_ptr<Interface>>;

	Children children_;

};

} // namespace dormouse_engine::reflection

#endif /* _DORMOUSEENGINE_REFLECTION_INTERFACE_HPP_ */
