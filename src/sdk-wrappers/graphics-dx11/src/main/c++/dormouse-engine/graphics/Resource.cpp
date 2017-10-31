#include "graphics.pch.hpp"

#include "Resource.hpp"

#include "detail/Internals.hpp"
#include "Device.hpp"
#include "DirectXError.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

namespace /* anonymous */
{

system::windows::COMWrapper<ID3D11ShaderResourceView> createSRV(
	ID3D11Device& dxDevice,
	ID3D11Resource& dxResource,
	D3D11_SHADER_RESOURCE_VIEW_DESC* desc
	)
{
	auto result = system::windows::COMWrapper<ID3D11ShaderResourceView>();

	checkDirectXCall(
		dxDevice.CreateShaderResourceView(&dxResource, desc, &result.get()),
		"Failed to create a shader resource view"
		);

	return result;
}

system::windows::COMWrapper<ID3D11UnorderedAccessView> createUAV(
	ID3D11Device& dxDevice,
	ID3D11Resource& dxResource,
	const D3D11_UNORDERED_ACCESS_VIEW_DESC& desc
	)
{
	auto result = system::windows::COMWrapper<ID3D11UnorderedAccessView>();

	checkDirectXCall(
		dxDevice.CreateUnorderedAccessView(&dxResource, &desc, &result.get()),
		"Failed to create an unordered access view"
		);

	return result;
}

} // anonymous namespace

ResourceView::ResourceView(const Texture& texture) {
	resourceView_ = createSRV(
		detail::Internals::dxDevice(texture),
		*detail::Internals::dxResourcePtr(texture),
		nullptr
		);
}

ResourceView::ResourceView(const Buffer& buffer, PixelFormat elementFormat) {
	auto& dxBuffer = static_cast<ID3D11Buffer&>(*detail::Internals::dxResourcePtr(buffer));

	auto bufferDesc = D3D11_BUFFER_DESC();
	dxBuffer.GetDesc(&bufferDesc);

	auto srvDesc = D3D11_SHADER_RESOURCE_VIEW_DESC();
	std::memset(&srvDesc, 0, sizeof(decltype(srvDesc)));

	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Format = static_cast<DXGI_FORMAT>(elementFormat.id());
	srvDesc.Buffer.ElementOffset = 0;
	srvDesc.Buffer.NumElements = static_cast<UINT>(bufferDesc.ByteWidth / elementFormat.pixelSize());

	resourceView_ = createSRV(
		detail::Internals::dxDevice(buffer),
		dxBuffer,
		&srvDesc
		);
}

Resource::Id ResourceView::resourceId() const {
	auto* dxResource = static_cast<ID3D11Resource*>(nullptr);
	resourceView_->GetResource(&dxResource);
	return reinterpret_cast<std::uintptr_t>(dxResource);
}

UnorderedAccessView::UnorderedAccessView(
	const Buffer& buffer,
	PixelFormat elementFormat,
	size_t firstElementIdx,
	size_t elementCount
	)
{
	auto desc = D3D11_UNORDERED_ACCESS_VIEW_DESC();
	std::memset(&desc, 0, sizeof(desc));

	desc.Format = static_cast<DXGI_FORMAT>(elementFormat.id());
	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = static_cast<UINT>(firstElementIdx);
	desc.Buffer.NumElements = static_cast<UINT>(elementCount);

	uav_ = createUAV(
		detail::Internals::dxDevice(buffer),
		*detail::Internals::dxResourcePtr(buffer),
		desc
		);
}
