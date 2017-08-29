#include "graphics.pch.hpp"

#include "Resource.hpp"

#include "Device.hpp"
#include "DirectXError.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"

using namespace dormouse_engine::graphics;

ResourceView::ResourceView(Device& device, const Texture& texture) {
	checkDirectXCall(
		device.internalDevice().CreateShaderResourceView(
			texture.internalResource(), nullptr, &resourceView_.get()),
		"Failed to create a resource view"
		);
}

ResourceView::ResourceView(Device& device, const Buffer& buffer, PixelFormat elementFormat) {
	auto bufferDesc = D3D11_BUFFER_DESC();
	static_cast<ID3D11Buffer*>(buffer.internalResource().get())->GetDesc(&bufferDesc);

	auto srvDesc = D3D11_SHADER_RESOURCE_VIEW_DESC();
	std::memset(&srvDesc, 0, sizeof(decltype(srvDesc)));

	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Format = static_cast<DXGI_FORMAT>(elementFormat.id());
	srvDesc.Buffer.ElementOffset = 0;
	srvDesc.Buffer.NumElements = static_cast<UINT>(bufferDesc.ByteWidth / elementFormat.pixelSize());

	checkDirectXCall(
		device.internalDevice().CreateShaderResourceView(
			buffer.internalResource(), &srvDesc, &resourceView_.get()),
		"Failed to create a shader resource view of buffer"
		);
}
