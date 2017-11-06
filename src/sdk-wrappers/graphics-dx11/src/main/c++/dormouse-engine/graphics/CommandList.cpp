#include "graphics.pch.hpp"

#include "CommandList.hpp"

#include "dormouse-engine/exceptions/LogicError.hpp"

#include "detail/Internals.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"
#include "Device.hpp"
#include "RenderState.hpp"
#include "Resource.hpp"
#include "Sampler.hpp"
#include "DirectXError.hpp"
#include "ShaderType.hpp"
#include "PixelFormat.hpp"
#include "InputLayout.hpp"
#include "Viewport.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

CommandList::CommandList(system::windows::COMWrapper<ID3D11DeviceContext> internalDeviceContext) noexcept :
	deviceContext_(std::move(internalDeviceContext))
{
}

void CommandList::draw(size_t startingIndex, size_t vertexCount, PrimitiveTopology primitiveTopology) noexcept {
	deviceContext_->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(primitiveTopology));
	deviceContext_->Draw(static_cast<UINT>(vertexCount), static_cast<UINT>(startingIndex));
}

void CommandList::drawIndexed(size_t startingIndex, size_t indexCount, PrimitiveTopology primitiveTopology) noexcept {
	deviceContext_->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(primitiveTopology));
	deviceContext_->DrawIndexed(static_cast<UINT>(indexCount), static_cast<UINT>(startingIndex), 0);
}

void CommandList::drawIndexedInstanced(size_t vertexCountPerInstance, size_t instanceCount,
	size_t startingIndex, PrimitiveTopology primitiveTopology) noexcept
{
	deviceContext_->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(primitiveTopology));
	deviceContext_->DrawIndexedInstanced(
		static_cast<UINT>(vertexCountPerInstance),
		static_cast<UINT>(instanceCount),
		static_cast<UINT>(startingIndex),
		0,
		0u
		);
}

void CommandList::dispatch(size_t threadGroupsX, size_t threadGroupsY, size_t threadGroupsZ) noexcept {
	deviceContext_->Dispatch(
		static_cast<UINT>(threadGroupsX),
		static_cast<UINT>(threadGroupsY),
		static_cast<UINT>(threadGroupsZ)
		);
}

CommandList::LockedData CommandList::lock(const Resource& data, LockPurpose lockPurpose) {
	auto* dxResource = detail::Internals::dxResourcePtr(data);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	checkDirectXCall(
		deviceContext_->Map(dxResource, 0, static_cast<D3D11_MAP>(lockPurpose), 0, &mappedResource),
		"Failed to map the provided resource"
		);


	auto result = LockedData();

	result.pixels = LockedData::Pixels(
		reinterpret_cast<essentials::Byte*>(mappedResource.pData),
		[deviceContext = deviceContext_, dxResource](essentials::Byte*) {
			deviceContext->Unmap(dxResource, 0);
		}
		);

	result.rowPitch = mappedResource.RowPitch;
	result.depthPitch = mappedResource.DepthPitch;

	return result;
}

void CommandList::copy(const Resource& source, const Resource& target) {
	deviceContext_->CopyResource(
		detail::Internals::dxResourcePtr(target),
		detail::Internals::dxResourcePtr(source)
		);
}

void CommandList::setRenderTarget(const RenderTargetView& renderTarget, const DepthStencilView& depthStencil) {
	auto* dxRTV = &detail::Internals::dxRenderTargetView(renderTarget);
	auto* dxDSV = detail::Internals::dxDepthStencilView(depthStencil);
	deviceContext_->OMSetRenderTargets(1, &dxRTV, dxDSV);
}

void CommandList::setViewport(const Viewport& viewport) {
	deviceContext_->RSSetViewports(1, &detail::Internals::dxViewport(viewport));
}

void CommandList::setScissorRect(const ScissorRect& scissorRect) {
	deviceContext_->RSSetScissorRects(1, detail::Internals::dxScissorRect(scissorRect));
}

void CommandList::setInputLayout(const InputLayout& inputLayout) noexcept {
	deviceContext_->IASetInputLayout(detail::Internals::dxInputLayoutPtr(inputLayout));
}

void CommandList::setShader(const VertexShader& vertexShader) noexcept {
	deviceContext_->VSSetShader(detail::Internals::dxVertexShaderPtr(vertexShader), nullptr, 0);
}

void CommandList::setShader(const GeometryShader& geometryShader) noexcept {
	deviceContext_->GSSetShader(detail::Internals::dxGeometryShaderPtr(geometryShader), nullptr, 0);
}

void CommandList::setShader(const HullShader& hullShader) noexcept {
	deviceContext_->HSSetShader(detail::Internals::dxHullShaderPtr(hullShader), nullptr, 0);
}

void CommandList::setShader(const DomainShader& domainShader) noexcept {
	deviceContext_->DSSetShader(detail::Internals::dxDomainShaderPtr(domainShader), nullptr, 0);
}

void CommandList::setShader(const PixelShader& pixelShader) noexcept {
	deviceContext_->PSSetShader(detail::Internals::dxPixelShaderPtr(pixelShader), nullptr, 0);
}

void CommandList::setShader(const ComputeShader& computeShader) noexcept {
	deviceContext_->CSSetShader(detail::Internals::dxComputeShaderPtr(computeShader), nullptr, 0);
}

void CommandList::setConstantBuffer(const Buffer& buffer, ShaderType stage, size_t slot) {
	auto* buf = static_cast<ID3D11Buffer*>(detail::Internals::dxResourcePtr(buffer));

	switch (stage) {
	case ShaderType::VERTEX:
		deviceContext_->VSSetConstantBuffers(static_cast<UINT>(slot), 1, &buf);
		break;
	case ShaderType::GEOMETRY:
		deviceContext_->GSSetConstantBuffers(static_cast<UINT>(slot), 1, &buf);
		break;
	case ShaderType::HULL:
		deviceContext_->HSSetConstantBuffers(static_cast<UINT>(slot), 1, &buf);
		break;
	case ShaderType::DOMAIN:
		deviceContext_->DSSetConstantBuffers(static_cast<UINT>(slot), 1, &buf);
		break;
	case ShaderType::PIXEL:
		deviceContext_->PSSetConstantBuffers(static_cast<UINT>(slot), 1, &buf);
		break;
	case ShaderType::COMPUTE:
		deviceContext_->CSSetConstantBuffers(static_cast<UINT>(slot), 1, &buf);
		break;
	default:
		throw dormouse_engine::exceptions::LogicError("Unknown shader type: " + toString(stage));
	}
}

void CommandList::setIndexBuffer(const Buffer& buffer, size_t offset, size_t stride) {
	auto* buf = static_cast<ID3D11Buffer*>(detail::Internals::dxResourcePtr(buffer));

	auto format = DXGI_FORMAT();
	
	if (buf != nullptr) {
		assert(stride == 2 || stride == 4);

		if (stride == 2) {
			format = DXGI_FORMAT_R16_UINT;
		} else if (stride == 4) {
			format = DXGI_FORMAT_R32_UINT;
		}
	}

	deviceContext_->IASetIndexBuffer(buf, format, static_cast<UINT>(offset));
}

void CommandList::setVertexBuffer(const Buffer& buffer, size_t slot, size_t stride) {
	auto* buf = static_cast<ID3D11Buffer*>(detail::Internals::dxResourcePtr(buffer));
	
	auto strideParam = static_cast<UINT>(stride);
	auto offsetParam = 0u;

	deviceContext_->IASetVertexBuffers(static_cast<UINT>(slot), 1, &buf, &strideParam, &offsetParam);
}

void CommandList::setResource(const ResourceView& resourceView, ShaderType stage, size_t slot) {
	auto* srv = detail::Internals::dxResourceViewPtr(resourceView);

	switch (stage) {
	case ShaderType::VERTEX:
		deviceContext_->VSSetShaderResources(static_cast<UINT>(slot), 1, &srv);
		break;
	case ShaderType::GEOMETRY:
		deviceContext_->GSSetShaderResources(static_cast<UINT>(slot), 1, &srv);
		break;
	case ShaderType::HULL:
		deviceContext_->HSSetShaderResources(static_cast<UINT>(slot), 1, &srv);
		break;
	case ShaderType::DOMAIN:
		deviceContext_->DSSetShaderResources(static_cast<UINT>(slot), 1, &srv);
		break;
	case ShaderType::PIXEL:
		deviceContext_->PSSetShaderResources(static_cast<UINT>(slot), 1, &srv);
		break;
	case ShaderType::COMPUTE:
		deviceContext_->CSSetShaderResources(static_cast<UINT>(slot), 1, &srv);
		break;
	default:
		throw dormouse_engine::exceptions::LogicError("Unknown shader type: " + toString(stage));
	}
}

void CommandList::setUnorderedAccessView(const UnorderedAccessView& unorderedAccessView, size_t slot) noexcept {
	auto* uav = detail::Internals::dxUnorderedAccessViewPtr(unorderedAccessView);
	deviceContext_->CSSetUnorderedAccessViews(static_cast<UINT>(slot), 1, &uav, nullptr);
}

void CommandList::setSampler(const Sampler& sampler, ShaderType stage, size_t slot) {
	auto* ss = detail::Internals::dxSamplerStatePtr(sampler);

	switch (stage) {
	case ShaderType::VERTEX:
		deviceContext_->VSSetSamplers(static_cast<UINT>(slot), 1, &ss);
		break;
	case ShaderType::GEOMETRY:
		deviceContext_->GSSetSamplers(static_cast<UINT>(slot), 1, &ss);
		break;
	case ShaderType::HULL:
		deviceContext_->HSSetSamplers(static_cast<UINT>(slot), 1, &ss);
		break;
	case ShaderType::DOMAIN:
		deviceContext_->DSSetSamplers(static_cast<UINT>(slot), 1, &ss);
		break;
	case ShaderType::PIXEL:
		deviceContext_->PSSetSamplers(static_cast<UINT>(slot), 1, &ss);
		break;
	case ShaderType::COMPUTE:
		deviceContext_->CSSetSamplers(static_cast<UINT>(slot), 1, &ss);
		break;
	default:
		throw dormouse_engine::exceptions::LogicError("Unknown shader type: " + toString(stage));
	}
}

void CommandList::setRenderState(const RenderState& renderState) {
	deviceContext_->RSSetState(&detail::Internals::dxRasteriserState(renderState));
	deviceContext_->OMSetBlendState(&detail::Internals::dxBlendState(renderState), 0, 0xffffffff); // TODO: args
}
