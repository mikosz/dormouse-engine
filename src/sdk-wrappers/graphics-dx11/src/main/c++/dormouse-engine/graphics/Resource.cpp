#include "graphics.pch.hpp"

#include "Resource.hpp"

#include "detail/Internals.hpp"
#include "Device.hpp"
#include "DirectXError.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"

using namespace dormouse_engine::graphics;

ResourceView::ResourceView(const Texture& texture) {
	checkDirectXCall(
		detail::Internals::dxDevice(texture).CreateShaderResourceView(
			&detail::Internals::dxResource(texture), nullptr, &resourceView_.get()),
		"Failed to create a resource view"
		);
}

ResourceView::ResourceView(const Buffer& buffer, PixelFormat elementFormat) {
	auto& dxBuffer = static_cast<ID3D11Buffer&>(detail::Internals::dxResource(buffer));

	auto bufferDesc = D3D11_BUFFER_DESC();
	dxBuffer.GetDesc(&bufferDesc);

	auto srvDesc = D3D11_SHADER_RESOURCE_VIEW_DESC();
	std::memset(&srvDesc, 0, sizeof(decltype(srvDesc)));

	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Format = static_cast<DXGI_FORMAT>(elementFormat.id());
	srvDesc.Buffer.ElementOffset = 0;
	srvDesc.Buffer.NumElements = static_cast<UINT>(bufferDesc.ByteWidth / elementFormat.pixelSize());

	checkDirectXCall(
		detail::Internals::dxDevice(buffer).CreateShaderResourceView(&dxBuffer, &srvDesc, &resourceView_.get()),
		"Failed to create a shader resource view of buffer"
		);
}

Resource::Id ResourceView::resourceId() const {
	auto* dxResource = static_cast<ID3D11Resource*>(nullptr);
	resourceView_->GetResource(&dxResource);
	return reinterpret_cast<std::uintptr_t>(dxResource);
}
