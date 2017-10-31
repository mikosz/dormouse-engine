#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_COMMANDLIST_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_COMMANDLIST_HPP_

#include <cstdint>
#include <memory>
#include <functional>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "detail/detailfwd.hpp"
#include "PrimitiveTopology.hpp"
#include "Shader.hpp"
#include "ShaderType.hpp"

namespace dormouse_engine::graphics {

class Buffer;
class Resource;
class Device;
class Texture;
class Sampler;
class RenderState;
class InputLayout;
class Viewport;
class ScissorRect;
class PixelFormat;
class RenderTargetView;
class DepthStencilView;
class ResourceView;
class UnorderedAccessView;

class CommandList {
public:

	struct LockedData {
		using Pixels = std::unique_ptr<essentials::Byte, std::function<void(essentials::Byte*)>>;

		Pixels pixels;
		size_t rowPitch;
		size_t depthPitch;
	};

	enum class LockPurpose {
		WRITE_DISCARD = D3D11_MAP_WRITE_DISCARD,
		WRITE_NO_OVERWRITE = D3D11_MAP_WRITE_NO_OVERWRITE,
		READ = D3D11_MAP_READ,
	};

	CommandList() = default;

	CommandList(system::windows::COMWrapper<ID3D11DeviceContext> internalDeviceContext) noexcept;

	void initialise(system::windows::COMWrapper<ID3D11DeviceContext> internalDeviceContext) noexcept;

	void draw(size_t startingIndex, size_t vertexCount, PrimitiveTopology primitiveTopology) noexcept;

	void drawIndexed(size_t startingIndex, size_t indexCount, PrimitiveTopology primitiveTopology) noexcept;

	void drawIndexedInstanced(size_t vertexCountPerInstance, size_t instanceCount, size_t startingIndex,
		PrimitiveTopology primitiveTopology) noexcept;

	void dispatch(size_t threadsX, size_t threadsY, size_t threadsZ) noexcept;

	LockedData lock(const Resource& data, LockPurpose lockPurpose);

	void copy(const Resource& source, const Resource& target);

	void setRenderTarget(const RenderTargetView& renderTarget, const DepthStencilView& depthStencil);

	void setViewport(const Viewport& viewport);

	void setScissorRect(const ScissorRect& scissorRect);

	void setInputLayout(const InputLayout& inputLayout) noexcept;

	void setShader(const VertexShader& vertexShader) noexcept;

	void setShader(const GeometryShader& geometryShader) noexcept;

	void setShader(const HullShader& hullShader) noexcept;

	void setShader(const DomainShader& domainShader) noexcept;

	void setShader(const PixelShader& pixelShader) noexcept;

	void setShader(const ComputeShader& computeShader) noexcept;

	void setConstantBuffer(const Buffer& buffer, ShaderType stage, size_t slot);

	void setIndexBuffer(const Buffer& buffer, size_t offset, size_t stride);

	void setVertexBuffer(const Buffer& buffer, size_t slot, size_t stride);

	void setResource(const ResourceView& resource, ShaderType stage, size_t slot);

	void setUnorderedAccessView(const UnorderedAccessView& unorderedAccessView, size_t slot) noexcept;

	void setSampler(const Sampler& sampler, ShaderType stage, size_t slot);

	void setRenderState(const RenderState& renderState);

private:

	system::windows::COMWrapper<ID3D11DeviceContext> deviceContext_;

	friend struct detail::Internals;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_COMMANDLIST_HPP_ */
