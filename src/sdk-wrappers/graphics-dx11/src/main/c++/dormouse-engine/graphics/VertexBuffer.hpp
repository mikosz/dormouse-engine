#ifndef _DORMOUSEENGINE_GRAPHICS_VERTEXBUFFER_HPP_
#define _DORMOUSEENGINE_GRAPHICS_VERTEXBUFFER_HPP_

#include "Buffer.hpp"

namespace dormouse_engine {
namespace graphics {

class VertexBuffer : public Buffer {
public:

	VertexBuffer() = default;

	VertexBuffer(Device& renderer, const Configuration& configuration, const void* initialData = 0) :
		Buffer(renderer, CreationPurpose::VERTEX_BUFFER, configuration, initialData),
		stride_(configuration.stride)
	{
	}

	size_t stride() const {
		return stride_;
	}

private:

	size_t stride_ = 0;

};

} // namespace graphics
} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_GRAPHICS_VERTEXBUFFER_HPP_ */
