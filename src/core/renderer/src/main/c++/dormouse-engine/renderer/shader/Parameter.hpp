#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PARAMETER_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PARAMETER_HPP_

#include "dormouse-engine/graphics/PixelFormat.hpp"
#include "PropertyId.hpp"

namespace dormouse_engine::renderer::shader {

class Property;

class Parameter {
public:

	Parameter(PropertyDescriptor propertyDescriptor, graphics::PixelFormat format, size_t offset) noexcept :
		propertyDescriptor_(std::move(propertyDescriptor)),
		format_(format),
		offset_(offset)
	{
	}

private:

	PropertyDescriptor propertyDescriptor_;

	graphics::PixelFormat format_;

	size_t offset_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PARAMETER_HPP_ */
