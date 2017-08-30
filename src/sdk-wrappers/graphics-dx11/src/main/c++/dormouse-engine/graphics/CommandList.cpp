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

CommandList::CommandList() {
}

CommandList::CommandList(system::windows::COMWrapper<ID3D11DeviceContext> internalDeviceContext) :
	deviceContext_(std::move(internalDeviceContext))
{
}

void CommandList::initialise(system::windows::COMWrapper<ID3D11DeviceContext> internalDeviceContext) {
	deviceContext_ = internalDeviceContext;
}

void CommandList::drawIndexed(size_t startingIndex, size_t indexCount, PrimitiveTopology primitiveTopology) {
	deviceContext_->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(primitiveTopology));
	deviceContext_->DrawIndexed(static_cast<UINT>(indexCount), static_cast<UINT>(startingIndex), 0);
}

void CommandList::drawIndexedInstanced(size_t vertexCountPerInstance, size_t instanceCount,
	size_t startingIndex, PrimitiveTopology primitiveTopology)
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

CommandList::LockedData CommandList::lock(Resource& data, LockPurpose lockPurpose) {
	auto& dxResource = detail::Internals::dxResource(data);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	checkDirectXCall(
		deviceContext_->Map(&dxResource, 0, static_cast<D3D11_MAP>(lockPurpose), 0, &mappedResource),
		"Failed to map the provided resource"
		);

	return LockedData(
		reinterpret_cast<std::uint8_t*>(mappedResource.pData),
		[deviceContext = deviceContext_, &dxResource](std::uint8_t*) {
			deviceContext->Unmap(&dxResource, 0);
		}
		);
}

void CommandList::setRenderTarget(const RenderTargetView& renderTarget, const DepthStencilView& depthStencil) {
	auto* dxRTV = &detail::Internals::dxRenderTargetView(renderTarget);
	auto& dxDSV = detail::Internals::dxDepthStencilView(depthStencil);
	deviceContext_->OMSetRenderTargets(1, &dxRTV, &dxDSV);
}

void CommandList::setViewport(Viewport& viewport) {
	deviceContext_->RSSetViewports(1, &detail::Internals::dxViewport(viewport));
}

void CommandList::setInputLayout(const InputLayout* inputLayout) noexcept {
	deviceContext_->IASetInputLayout(&detail::Internals::dxInputLayout(*inputLayout));
}

void CommandList::setVertexShader(VertexShader* vertexShader) noexcept {
	deviceContext_->VSSetShader(&vertexShader->internalShader(), nullptr, 0);
}

void CommandList::setGeometryShader(GeometryShader* geometryShader) noexcept {
	if (geometryShader != nullptr) {
		deviceContext_->GSSetShader(&geometryShader->internalShader(), nullptr, 0);
	} else {
		deviceContext_->GSSetShader(nullptr, nullptr, 0);
	}
}

void CommandList::setHullShader(HullShader* hullShader) noexcept {
	if (hullShader != nullptr) {
		deviceContext_->HSSetShader(&hullShader->internalShader(), nullptr, 0);
	}
	else {
		deviceContext_->HSSetShader(nullptr, nullptr, 0);
	}
}

void CommandList::setDomainShader(DomainShader* domainShader) noexcept {
	if (domainShader != nullptr) {
		deviceContext_->DSSetShader(&domainShader->internalShader(), nullptr, 0);
	}
	else {
		deviceContext_->DSSetShader(nullptr, nullptr, 0);
	}
}

void CommandList::setPixelShader(PixelShader* pixelShader) noexcept {
	deviceContext_->PSSetShader(&pixelShader->internalShader(), nullptr, 0);
}

void CommandList::setConstantBuffer(Buffer& buffer, ShaderType stage, size_t slot) {
	auto* buf = static_cast<ID3D11Buffer*>(&detail::Internals::dxResource(buffer));

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
	default:
		throw dormouse_engine::exceptions::LogicError("Unknown shader type: " + toString(stage));
	}
}

void CommandList::setIndexBuffer(const Buffer& buffer, size_t offset) {
	auto* buf = static_cast<ID3D11Buffer*>(&detail::Internals::dxResource(buffer));

	auto desc = D3D11_BUFFER_DESC();
	buf->GetDesc(&desc);

	auto format = DXGI_FORMAT();
	if (desc.StructureByteStride == 2) {
		format = DXGI_FORMAT_R16_UINT;
	} else if (desc.StructureByteStride == 4) {
		format = DXGI_FORMAT_R32_UINT;
	} else {
		throw dormouse_engine::exceptions::LogicError(
			"Unexpected byte stride for index buffer: " + std::to_string(desc.StructureByteStride));
	}

	deviceContext_->IASetIndexBuffer(buf, format, static_cast<UINT>(offset));
}

void CommandList::setVertexBuffer(const Buffer& buffer, size_t slot) {
	auto* buf = static_cast<ID3D11Buffer*>(&detail::Internals::dxResource(buffer));

	auto desc = D3D11_BUFFER_DESC();
	buf->GetDesc(&desc);

	auto strideParam = desc.StructureByteStride;
	auto offsetParam = 0u;

	deviceContext_->IASetVertexBuffers(static_cast<UINT>(slot), 1, &buf, &strideParam, &offsetParam);
}

void CommandList::setResource(const ResourceView& resourceView, ShaderType stage, size_t slot) {
	auto* srv = &detail::Internals::dxResourceView(resourceView);

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
	default:
		throw dormouse_engine::exceptions::LogicError("Unknown shader type: " + toString(stage));
	}
}

void CommandList::setSampler(const Sampler& sampler, ShaderType stage, size_t slot) {
	auto ss = &detail::Internals::dxSamplerState(sampler);

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
	default:
		throw dormouse_engine::exceptions::LogicError("Unknown shader type: " + toString(stage));
	}
}

void CommandList::setRenderState(const RenderState& renderState) {
	deviceContext_->RSSetState(&detail::Internals::dxRasteriserState(renderState));
	deviceContext_->OMSetBlendState(&detail::Internals::dxBlendState(renderState), 0, 0xffffffff); // TODO: args
}
