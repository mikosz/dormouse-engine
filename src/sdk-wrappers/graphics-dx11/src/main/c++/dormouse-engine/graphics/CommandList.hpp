#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_COMMANDLIST_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_COMMANDLIST_HPP_

#include <cstdint>
#include <memory>
#include <functional>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "detail/detailfwd.hpp"
#include "PrimitiveTopology.hpp"
#include "Shader.hpp"
#include "ShaderType.hpp"

namespace dormouse_engine::graphics {

class Buffer;
class Resource;
class Device;
class ConstantBuffer;
class IndexBuffer;
class VertexBuffer;
class Texture;
class Sampler;
class RenderState;
class InputLayout;
class Viewport;
class PixelFormat;
class RenderTargetView;
class DepthStencilView;
class ResourceView;

class CommandList {
public:

	using LockedData = std::unique_ptr<std::uint8_t, std::function<void(std::uint8_t*)>>;

	enum class LockPurpose {
		WRITE_DISCARD = D3D11_MAP_WRITE_DISCARD,
		WRITE_NO_OVERWRITE = D3D11_MAP_WRITE_NO_OVERWRITE,
	};

	CommandList() = default;

	CommandList(system::windows::COMWrapper<ID3D11DeviceContext> internalDeviceContext);

	void initialise(system::windows::COMWrapper<ID3D11DeviceContext> internalDeviceContext);

	void draw(size_t startingIndex, size_t vertexCount, PrimitiveTopology primitiveTopology);

	void drawIndexed(size_t startingIndex, size_t indexCount, PrimitiveTopology primitiveTopology);

	void drawIndexedInstanced(size_t vertexCountPerInstance, size_t instanceCount, size_t startingIndex,
		PrimitiveTopology primitiveTopology);

	LockedData lock(Resource& data, LockPurpose lockPurpose);

	void setRenderTarget(const RenderTargetView& renderTarget, const DepthStencilView& depthStencil);

	void setViewport(const Viewport& viewport);

	void setInputLayout(const InputLayout& inputLayout) noexcept;

	void setShader(const VertexShader& vertexShader) noexcept;

	void setShader(const GeometryShader& geometryShader) noexcept;

	void setShader(const HullShader& hullShader) noexcept;

	void setShader(const DomainShader& domainShader) noexcept;

	void setShader(const PixelShader& pixelShader) noexcept;

	void setConstantBuffer(Buffer& buffer, ShaderType stage, size_t slot);

	void setIndexBuffer(const Buffer& buffer, size_t offset);

	void setVertexBuffer(const Buffer& buffer, size_t slot);

	void setResource(const ResourceView& resource, ShaderType stage, size_t slot);

	void setSampler(const Sampler& sampler, ShaderType stage, size_t slot);

	void setRenderState(const RenderState& renderState);

private:

	system::windows::COMWrapper<ID3D11DeviceContext> deviceContext_;

	friend struct detail::Internals;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_COMMANDLIST_HPP_ */
