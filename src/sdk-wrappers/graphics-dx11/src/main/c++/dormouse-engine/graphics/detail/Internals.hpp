#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_DETAIL_INTERNALS_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_DETAIL_INTERNALS_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "../DirectXError.hpp"
#include "../Adapter.hpp"
#include "../Device.hpp"
#include "../CommandList.hpp"
#include "../Resource.hpp"
#include "../Texture.hpp"
#include "../Viewport.hpp"
#include "../ScissorRect.hpp"
#include "../InputLayout.hpp"
#include "../Sampler.hpp"
#include "../RenderState.hpp"
#include "../Shader.hpp"

namespace dormouse_engine::graphics::detail {

struct Internals {

	static system::windows::COMWrapper<IDXGIFactory> createDXGIFactory() {
		system::windows::COMWrapper<IDXGIFactory> factory;
		checkDirectXCall(
			CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory.get())),
			"Failed to create a DXGIFactory"
			);

		return factory;
	}

	static Texture createTextureFromDX11Texture(system::windows::COMWrapper<ID3D11Texture2D> texture) {
		return Texture(std::move(texture));
	}

	static IDXGIAdapter& dxAdapter(const Adapter& adapter) {
		return *adapter.adapter_;
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

	static ID3D11Resource* dxResourcePtr(const Resource& resource) {
		return resource.resource_.get();
	}

	static ID3D11ShaderResourceView* dxResourceViewPtr(const ResourceView& resourceView) {
		return resourceView.resourceView_.get();
	}

	static ID3D11UnorderedAccessView* dxUnorderedAccessViewPtr(const UnorderedAccessView& unorderedAccessView) {
		return unorderedAccessView.uav_.get();
	}

	static ID3D11RenderTargetView& dxRenderTargetView(const RenderTargetView& rtv) {
		return *rtv.renderTargetView_;
	}

	static ID3D11DepthStencilView* dxDepthStencilView(const DepthStencilView& rtv) {
		return rtv.depthStencilView_.get();
	}

	static const D3D11_VIEWPORT& dxViewport(const Viewport& viewport) {
		return viewport.viewport_;
	}

	static const D3D11_RECT* dxScissorRect(const ScissorRect& scissorRect) {
		if (scissorRect.enabled_) {
			return &scissorRect.scissorRect_;
		} else {
			return nullptr;
		}
	}

	static ID3D11InputLayout* dxInputLayoutPtr(const InputLayout& inputLayout) {
		return inputLayout.inputLayout_.get();
	}

	static ID3D11SamplerState* dxSamplerStatePtr(const Sampler& sampler) {
		return sampler.samplerState_.get();
	}

	static ID3D11RasterizerState& dxRasteriserState(const RenderState& renderState) {
		return *renderState.rasteriserState_;
	}

	static ID3D11BlendState& dxBlendState(const RenderState& renderState) {
		return *renderState.blendState_;
	}

	static ID3D11VertexShader* dxVertexShaderPtr(const VertexShader& vertexShader) {
		return vertexShader.shader_.get();
	}

	static ID3D11GeometryShader* dxGeometryShaderPtr(const GeometryShader& geometryShader) {
		return geometryShader.shader_.get();
	}

	static ID3D11DomainShader* dxDomainShaderPtr(const DomainShader& domainShader) {
		return domainShader.shader_.get();
	}

	static ID3D11HullShader* dxHullShaderPtr(const HullShader& hullShader) {
		return hullShader.shader_.get();
	}

	static ID3D11PixelShader* dxPixelShaderPtr(const PixelShader& pixelShader) {
		return pixelShader.shader_.get();
	}

	static ID3D11ComputeShader* dxComputeShaderPtr(const ComputeShader& computeShader) {
		return computeShader.shader_.get();
	}

};

} // namespace dormouse_engine::graphics::detail

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_DETAIL_INTERNALS_HPP_ */
