#include "graphics.pch.hpp"

#include "Buffer.hpp"

#include <cstring>
#include <stdexcept>
#include <functional>

#include "detail/Internals.hpp"
#include "Device.hpp"
#include "DirectXError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

namespace /* anonymous */ {

system::windows::COMWrapper<ID3D11Resource> createBufferResource(
	Device& device,
	const Buffer::Configuration& configuration,
	essentials::ConstBufferView initialData
	)
{
	auto desc = D3D11_BUFFER_DESC();
	std::memset(&desc, 0, sizeof(desc));

	desc.ByteWidth = static_cast<UINT>(configuration.size);
	desc.BindFlags = static_cast<UINT>(configuration.purpose);

	if (configuration.allowModifications) {
		if (configuration.allowCPURead) {
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		} else {
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
	} else {
		if (configuration.allowCPURead) {
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		} else {
			if (configuration.allowGPUWrite) {
				desc.Usage = D3D11_USAGE_DEFAULT;
			} else {
				desc.Usage = D3D11_USAGE_IMMUTABLE;
			}
		}
	}

	auto* dataPtr = static_cast<D3D11_SUBRESOURCE_DATA*>(nullptr);
	auto data = D3D11_SUBRESOURCE_DATA();
	if (initialData.data()) {
		std::memset(&data, 0, sizeof(data));
		data.pSysMem = initialData.data();
		dataPtr = &data;
	}

	auto buffer = system::windows::COMWrapper<ID3D11Buffer>();
	checkDirectXCall(detail::Internals::dxDevice(device).CreateBuffer(&desc, dataPtr, &buffer.get()),
		"Failed to create a buffer");

	return buffer;
}

} // anonymous namespace

Buffer::Buffer(Device& renderer, Configuration configuration, essentials::ConstBufferView initialData) :
	Resource(createBufferResource(renderer, configuration, initialData))
{
}
