#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PROPERTIES_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PROPERTIES_HPP_

#include <unordered_map>

#include "dormouse-engine/exceptions/RuntimeError.hpp"
#include "PropertyId.hpp"
#include "Property.hpp"

namespace dormouse_engine::renderer::shader {

class PropertyNotBound : public exceptions::RuntimeError {
public:

	PropertyNotBound(essentials::StringId id) :
		exceptions::RuntimeError("Required property \"" + id.string() + "\" is not bound")
	{
	}

};

class Properties {
public:

	void add(essentials::StringId id, Property child) {
		properties_[std::move(id)] = std::move(child);
	}

	const Property& get(essentials::StringId id) const;

private:

	using Children = std::unordered_map<essentials::StringId, Property>;

	Children properties_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PROPERTIES_HPP_ */
