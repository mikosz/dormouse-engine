#ifndef _DORMOUSEENGINE_GRAPHICS_BUFFER_HPP_
#define _DORMOUSEENGINE_GRAPHICS_BUFFER_HPP_

#include <functional>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"

#include "Resource.hpp"
#include "ShaderType.hpp"
#include "PixelFormat.hpp"

namespace dormouse_engine::graphics {

class Device;

class Buffer : public Resource {
public:

	enum class CreationPurpose {
		VERTEX_BUFFER = D3D11_BIND_VERTEX_BUFFER,
		INDEX_BUFFER = D3D11_BIND_INDEX_BUFFER,
		CONSTANT_BUFFER = D3D11_BIND_CONSTANT_BUFFER,
		SHADER_RESOURCE = D3D11_BIND_SHADER_RESOURCE, // TODO: duplicated with texture
	};

	struct Configuration {

		size_t size = 0;

		size_t stride = 0;

		bool allowModifications = false;

		bool allowCPURead = false;

		bool allowGPUWrite = false;

		// TODO: move to another argument? Use different constructor?
		PixelFormat elementFormat; // Used only when purpose is SHADER_RESOURCE

		Configuration() = default;

		Configuration(
			size_t size,
			size_t stride,
			bool allowModifications,
			bool allowCPURead,
			bool allowGPUWrite
			) :
			size(size),
			stride(stride),
			allowModifications(allowModifications),
			allowCPURead(allowCPURead),
			allowGPUWrite(allowGPUWrite)
		{
		}

	};

	Buffer() = default;

	Buffer(Device& renderer, CreationPurpose purpose, Configuration configuration, const void* initialData = nullptr);

	const Configuration& configuration() const noexcept {
		return configuration_;
	}

private:

	Configuration configuration_;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_BUFFER_HPP_ */
