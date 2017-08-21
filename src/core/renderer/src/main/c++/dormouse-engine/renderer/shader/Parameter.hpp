#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PARAMETER_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PARAMETER_HPP_

#include "Property.hpp"

namespace dormouse_engine::renderer::shader {

class Parameter {
public:

	Parameter(
		PropertyDescriptor propertyDescriptor,
		Property::DataType dataType,
		size_t offset
		) noexcept :
		propertyDescriptor_(std::move(propertyDescriptor)),
		dataType_(dataType),
		offset_(offset)
	{
	}

	void* write(void* buffer, const Properties& properties) const {
		return properties.writeData(
			reinterpret_cast<std::uint8_t*>(buffer) + offset_,
			propertyDescriptor_,
			dataType_
			);
	}

	const Property::DataType dataType() const noexcept {
		return dataType_;
	}

	const size_t offset() const noexcept {
		return offset_;
	}

private:

	PropertyDescriptor propertyDescriptor_;

	Property::DataType dataType_;

	size_t offset_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PARAMETER_HPP_ */
