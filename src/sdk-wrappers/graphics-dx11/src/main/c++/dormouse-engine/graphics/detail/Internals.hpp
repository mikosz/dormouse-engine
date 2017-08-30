#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_DETAIL_INTERNALS_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_DETAIL_INTERNALS_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "../Device.hpp"
#include "../CommandList.hpp"
#include "../Resource.hpp"
#include "../Texture.hpp"
#include "../Viewport.hpp"
#include "../InputLayout.hpp"
#include "../Sampler.hpp"
#include "../RenderState.hpp"

namespace dormouse_engine::graphics::detail {

struct Internals {

	static Texture createTextureFromDX11Texture(system::windows::COMWrapper<ID3D11Texture2D> texture) {
		return Texture(std::move(texture));
	}

	static ID3D11Device& dxDevice(const Device& device) {
		return *device.d3dDevice_;
	}

	static ID3D11Device& dxDevice(const Resource& resource) {
		auto* device = static_cast<ID3D11Device*>(nullptr);
		resource.resource_->GetDevice(&device);
		return *device;
	}

	static ID3D11DeviceContext& dxDeviceContext(const CommandList& commandList) {
		return *commandList.deviceContext_;
	}

	static ID3D11Resource& dxResource(const Resource& resource) {
		return *resource.resource_;
	}

	static ID3D11ShaderResourceView& dxResourceView(const ResourceView& resourceView) {
		return *resourceView.resourceView_;
	}

	static ID3D11RenderTargetView& dxRenderTargetView(const RenderTargetView& rtv) {
		return *rtv.renderTargetView_;
	}

	static ID3D11DepthStencilView& dxDepthStencilView(const DepthStencilView& rtv) {
		return *rtv.depthStencilView_;
	}

	static const D3D11_VIEWPORT& dxViewport(const Viewport& viewport) {
		return viewport.viewport_;
	}

	static ID3D11InputLayout& dxInputLayout(const InputLayout& inputLayout) {
		return *inputLayout.inputLayout_;
	}

	static ID3D11SamplerState& dxSamplerState(const Sampler& sampler) {
		return *sampler.samplerState_;
	}

	static ID3D11RasterizerState& dxRasteriserState(const RenderState& renderState) {
		return *renderState.rasteriserState_;
	}

	static ID3D11BlendState& dxBlendState(const RenderState& renderState) {
		return *renderState.blendState_;
	}

};

} // namespace dormouse_engine::graphics::detail

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_DETAIL_INTERNALS_HPP_ */
