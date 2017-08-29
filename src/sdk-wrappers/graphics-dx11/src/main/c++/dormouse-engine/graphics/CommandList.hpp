#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_COMMANDLIST_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_COMMANDLIST_HPP_

#include <cstdint>
#include <memory>
#include <functional>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"

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

	CommandList();

	CommandList(system::windows::COMWrapper<ID3D11DeviceContext> internalDeviceContext);

	void initialise(system::windows::COMWrapper<ID3D11DeviceContext> internalDeviceContext);

	void drawIndexed(size_t startingIndex, size_t indexCount, PrimitiveTopology primitiveTopology);

	void drawIndexedInstanced(size_t vertexCountPerInstance, size_t instanceCount, size_t startingIndex,
		PrimitiveTopology primitiveTopology);

	LockedData lock(Resource& data, LockPurpose lockPurpose);

	void setRenderTarget(const RenderTargetView& renderTarget, const DepthStencilView& depthStencil);

	void setViewport(Viewport& viewport);

	void setInputLayout(const InputLayout* inputLayout) noexcept;

	void setVertexShader(VertexShader* vertexShader) noexcept;

	void setGeometryShader(GeometryShader* geometryShader) noexcept;

	void setHullShader(HullShader* hullShader) noexcept;

	void setDomainShader(DomainShader* domainShader) noexcept;

	void setPixelShader(PixelShader* pixelShader) noexcept;

	void setConstantBuffer(Buffer& buffer, ShaderType stage, size_t slot);

	void setIndexBuffer(const Buffer& buffer, size_t offset);

	void setVertexBuffer(const Buffer& buffer, size_t slot);

	void setResource(const ResourceView& resource, ShaderType stage, size_t slot);

	void setSampler(const Sampler& sampler, ShaderType stage, size_t slot);

	void setRenderState(const RenderState& renderState);

	ID3D11DeviceContext& internalDeviceContext() {
		return *deviceContext_;
	}

private:

	system::windows::COMWrapper<ID3D11DeviceContext> deviceContext_;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_COMMANDLIST_HPP_ */
