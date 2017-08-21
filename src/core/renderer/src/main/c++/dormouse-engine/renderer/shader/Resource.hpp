#ifndef _DORMOUSEENGINE_RENDERER_SHADER_RESOURCE_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_RESOURCE_HPP_

#include "dormouse-engine/milk/graphics/ShaderType.hpp"
#include "Property.hpp"

namespace dormouse_engine::renderer::shader {

class Resource {
public:

	Resource(
		PropertyDescriptor propertyDescriptor,
		milk::graphics::ShaderReflection::ResourceInfo::Type type,
		milk::graphics::ShaderType stage,
		size_t slot
		) noexcept :
		propertyDescriptor_(std::move(propertyDescriptor)),
		type_(type),
		stage_(stage),
		slot_(slot)
	{
	}

	void bind(DrawCommand& drawCommand, const Properties& properties) const {
		properties.bindResource(drawCommand, propertyDescriptor_, type_, stage_, slot_);
	}

private:

	PropertyDescriptor propertyDescriptor_;

	milk::graphics::ShaderType stage_;

	milk::graphics::ShaderReflection::ResourceInfo::Type type_;

	size_t slot_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_RESOURCE_HPP_ */
