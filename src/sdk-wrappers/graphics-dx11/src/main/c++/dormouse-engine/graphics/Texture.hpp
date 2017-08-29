#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_TEXTURE_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_TEXTURE_HPP_

#include <type_traits>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/enums/Mask.hpp"
#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "PixelFormat.hpp"
#include "Resource.hpp"

namespace dormouse_engine::graphics {

class Device;

class Texture : public Resource {
public:

	DE_MEMBER_FLAG_VALUES(
		CreationPurpose,
		(SHADER_RESOURCE)(D3D11_BIND_SHADER_RESOURCE)
		(RENDER_TARGET)(D3D11_BIND_RENDER_TARGET)
		(DEPTH_STENCIL)(D3D11_BIND_DEPTH_STENCIL)
		);

	struct Configuration1d {

		size_t width;

		size_t arraySize = 1u;

		size_t mipLevels = 1u;

		PixelFormat pixelFormat;

		bool allowModifications;

		bool allowCPURead;

		bool allowGPUWrite;

		dormouse_engine::Mask<CreationPurpose> purposeFlags;

	};

	struct Configuration2d : Configuration1d {

		size_t height;

		size_t sampleCount = 1u;

		size_t sampleQuality = 0u;

	};

	Texture(
		Device& device,
		const Configuration1d& configuration,
		essentials::ConstBufferView initialData = essentials::ConstBufferView()
		);

	Texture(
		Device& device,
		const Configuration2d& configuration,
		essentials::ConstBufferView initialData = essentials::ConstBufferView()
		);

};

class RenderTargetView {
public:

	RenderTargetView() = default;

	RenderTargetView(Device& device, const Texture& texture);

	RenderTargetView(Device& device, system::windows::COMWrapper<ID3D11Texture2D> texture2d); // TODO: move to detail somehow?

	system::windows::COMWrapper<ID3D11RenderTargetView> internalRenderTargetView() const {
		return renderTargetView_;
	}

private:

	system::windows::COMWrapper<ID3D11RenderTargetView> renderTargetView_;

};

class DepthStencilView {
public:

	DepthStencilView() = default;

	DepthStencilView(Device& device, const Texture& texture);

	system::windows::COMWrapper<ID3D11DepthStencilView> internalDepthStencilView() const {
		return depthStencilView_;
	}

private:

	system::windows::COMWrapper<ID3D11DepthStencilView> depthStencilView_;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_TEXTURE_HPP_ */
