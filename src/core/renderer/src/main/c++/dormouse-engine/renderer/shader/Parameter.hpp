#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PARAMETER_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PARAMETER_HPP_

#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/graphics/ShaderDataType.hpp"
#include "dormouse-engine/graphics/PixelFormat.hpp"
#include "PropertyId.hpp"

namespace dormouse_engine::renderer::shader {

class Property;

class Parameter {
public:

	Parameter(PropertyDescriptor propertyDescriptor, graphics::ShaderDataType dataType, size_t offset) noexcept :
		propertyDescriptor_(std::move(propertyDescriptor)),
		dataType_(std::move(dataType)),
		offset_(offset)
	{
	}

	void write(essentials::BufferView buffer, const Property& root) const;

private:

	PropertyDescriptor propertyDescriptor_;

	graphics::ShaderDataType dataType_;

	size_t offset_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PARAMETER_HPP_ */
