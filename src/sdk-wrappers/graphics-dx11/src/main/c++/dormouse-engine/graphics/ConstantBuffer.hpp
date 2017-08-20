#ifndef _DORMOUSEENGINE_GRAPHICS_CONSTANTBUFFER_HPP_
#define _DORMOUSEENGINE_GRAPHICS_CONSTANTBUFFER_HPP_

#include "Buffer.hpp"

namespace dormouse_engine::graphics {

class ConstantBuffer : public Buffer { // TODO: this, Vertex and IndexBuffer classes are probably useless.
public:

	ConstantBuffer(Device& renderer, const Configuration& configuration, const void* initialData = 0) :
		Buffer(renderer, CreationPurpose::CONSTANT_BUFFER, configuration, initialData)
	{
	}

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_CONSTANTBUFFER_HPP_ */
