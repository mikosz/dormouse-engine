#include "graphics.pch.hpp"

#include "Device.hpp"

#include "detail/Internals.hpp"
#include "DirectXError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

namespace /* anonymous */ {

std::tuple<
	system::windows::COMWrapper<ID3D11Device>,
	system::windows::COMWrapper<ID3D11DeviceContext>
	> createDXDevice(const Adapter& adapter, bool debug)
{
	auto creationFlags = UINT(0);
	if (debug) {
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}

	auto device = system::windows::COMWrapper<ID3D11Device>();
	auto context = system::windows::COMWrapper<ID3D11DeviceContext>();

	checkDirectXCall(
		D3D11CreateDevice(
			&detail::Internals::dxAdapter(adapter),
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&device.get(),
			nullptr,
			&context.get()
			),
		"Failed to create a directx device"
		);

	return std::make_tuple(std::move(device), std::move(context));
}

} // anonymous namespace

Device::Device(const Adapter& adapter, bool debug) {
	auto immediateContext = system::windows::COMWrapper<ID3D11DeviceContext>();
	std::tie(d3dDevice_, immediateContext) = createDXDevice(adapter, debug);
	immediateCommandList_ = immediateContext;
}

Device::~Device() {
	for (const auto& handler : deviceDestroyedHandlers_) {
		handler();
	}
}

CommandList& Device::getImmediateCommandList() {
	return immediateCommandList_;
}

CommandList Device::createDeferredCommandList() {
	system::windows::COMWrapper<ID3D11DeviceContext> deferredContext;
	checkDirectXCall(
		d3dDevice_->CreateDeferredContext(0, &deferredContext.get()),
		"Failed to create a deferred context"
		);
	return CommandList(deferredContext);
}

Device::LockedData Device::lock(Resource& data, LockPurpose lockPurpose) {
	auto& dxDeviceContext = detail::Internals::dxDeviceContext(immediateCommandList_);
	auto& dxResource = *detail::Internals::dxResourcePtr(data);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	checkDirectXCall(
		dxDeviceContext.Map(
			&dxResource, 0, static_cast<D3D11_MAP>(lockPurpose), 0, &mappedResource),
		"Failed to map the provided resource"
		);

	return LockedData(
		mappedResource.pData,
		[&dxDeviceContext, &dxResource](void*) {
			dxDeviceContext.Unmap(&dxResource, 0);
		}
		);
}
