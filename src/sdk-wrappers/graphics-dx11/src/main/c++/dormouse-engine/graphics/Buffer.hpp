#ifndef _DORMOUSEENGINE_GRAPHICS_BUFFER_HPP_
#define _DORMOUSEENGINE_GRAPHICS_BUFFER_HPP_

#include <functional>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "dormouse-engine/essentials/memory.hpp"
#include "Resource.hpp"
#include "ShaderType.hpp"
#include "PixelFormat.hpp"

namespace dormouse_engine::graphics {

class Device;

const auto CONSTANT_BUFFER_SLOT_COUNT_PER_SHADER = D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT;

class Buffer : public Resource {
public:

	enum class Purpose {
		VERTEX_BUFFER = D3D11_BIND_VERTEX_BUFFER,
		INDEX_BUFFER = D3D11_BIND_INDEX_BUFFER,
		CONSTANT_BUFFER = D3D11_BIND_CONSTANT_BUFFER,
		SHADER_RESOURCE = D3D11_BIND_SHADER_RESOURCE,
		UNORDERED_ACCESS = D3D11_BIND_UNORDERED_ACCESS
	};

	struct Configuration {

		size_t size;

		bool allowCPUWrite;

		bool allowCPURead;

		bool allowGPUWrite;

		Purpose purpose;

	};

	Buffer() = default;

	Buffer(
		Device& device,
		const Configuration& configuration,
		essentials::ConstBufferView initialData = essentials::ConstBufferView()
		);

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_BUFFER_HPP_ */
