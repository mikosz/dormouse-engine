#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PROPERTIES_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PROPERTIES_HPP_

#include <unordered_map>

#include "dormouse-engine/exceptions/RuntimeError.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/graphics/ShaderType.hpp"
#include "../command/commandfwd.hpp"
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

	void addChild(essentials::StringId id, essentials::observer_ptr<Properties> child) {
		children_[std::move(id)] = std::move(child);
	}

	void registerResource(essentials::StringId id, ResourceProperty property) {
		resourceProperties_[std::move(id)] = std::move(property);
	}

	void bindResource(
		command::DrawCommand& cmd,
		const PropertyId& id,
		void* object,
		graphics::ShaderType stage,
		size_t slot
		) const;

private:

	using Children = std::unordered_map<essentials::StringId, essentials::observer_ptr<Properties>>;

	using ResourceProperties = std::unordered_map<essentials::StringId, ResourceProperty>;

	Children children_;

	ResourceProperties resourceProperties_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PROPERTIES_HPP_ */
