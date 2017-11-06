#include "graphics.pch.hpp"

#include "Device.hpp"

#include <algorithm>

#include "detail/Internals.hpp"
#include "DirectXError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

namespace /* anonymous */ {

system::windows::COMWrapper<IDXGIFactory> createDXGIFactory() {
	system::windows::COMWrapper<IDXGIFactory> factory;
	checkDirectXCall(
		CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory.get())),
		"Failed to create a DXGIFactory"
		);

	return factory;
}

void queryAdapterAndRefreshRate(
	IDXGIFactory& dxgiFactory,
	system::windows::COMWrapper<IDXGIAdapter>* adapter
	) {
	checkDirectXCall(dxgiFactory.EnumAdapters(0, &adapter->get()), "Failed to enumerate video cards");

	system::windows::COMWrapper<IDXGIOutput> output;
	checkDirectXCall((*adapter)->EnumOutputs(0, &output.get()), "Failed to enumerate outputs");

	auto displayModes = std::vector<DXGI_MODE_DESC>();
	auto modeCount = UINT();
	auto displayModeListResult = HRESULT();
	displayModeListResult = output->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeCount, nullptr);
	if (displayModeListResult == DXGI_ERROR_NOT_CURRENTLY_AVAILABLE) {
		modeCount = 0;
	} else {
		checkDirectXCall(displayModeListResult, "Failed to get display mode count");

		displayModes.resize(modeCount);
		checkDirectXCall(
			output->GetDisplayModeList(
				DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeCount, &displayModes.front()),
			"Failed to get display modes"
		);
	}
}

std::tuple<size_t, size_t> windowDimensions(system::windows::WindowHandle windowHandle) {
	auto clientRect = RECT();
	GetClientRect(windowHandle, &clientRect);

	return std::make_tuple(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
}

void createD3DDevice(
	system::windows::WindowHandle windowHandle,
	IDXGIFactory& dxgiFactory,
	const Device::Configuration& configuration,
	const DXGI_RATIONAL& refreshRate,
	system::windows::COMWrapper<IDXGISwapChain>* swapChain,
	system::windows::COMWrapper<ID3D11Device>* device,
	system::windows::COMWrapper<ID3D11DeviceContext>* deviceContext
	) {
	UINT creationFlags = 0;
	if (configuration.debugDevice) {
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}

	checkDirectXCall(
		D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&device->get(),
			nullptr,
			&deviceContext->get()
			),
		"Failed to create a directx device"
		);
}

Texture extractBackBuffer(IDXGISwapChain* swapChain) {
	auto texture = system::windows::COMWrapper<ID3D11Texture2D>();

	checkDirectXCall(
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture.get())),
		"Failed to extract the back buffer texture"
		);

	return detail::Internals::createTextureFromDX11Texture(std::move(texture));
}

} // anonymous namespace

Device::Device(system::windows::WindowHandle windowHandle, const Configuration& configuration) :
	configuration_(configuration)
{
	auto dxgiFactory = createDXGIFactory();

	auto screenWidth = size_t();
	auto screenHeight = size_t();
	std::tie(screenWidth, screenHeight) = windowDimensions(windowHandle);

	DXGI_RATIONAL refreshRate;
	queryAdapterAndRefreshRate(*dxgiFactory, &adapter_, &refreshRate, screenWidth, screenHeight);

	system::windows::COMWrapper<ID3D11DeviceContext> immediateContext;
	createD3DDevice(windowHandle, *dxgiFactory, configuration, refreshRate, &swapChain_, &d3dDevice_, &immediateContext);
	immediateCommandList_.initialise(immediateContext);

	backBuffer_ = extractBackBuffer(swapChain_);

	UINT quality;
	d3dDevice_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quality); // TODO: literal in code

	Texture::Configuration2d depthStencilConfig;
	depthStencilConfig.width = screenWidth;
	depthStencilConfig.height = screenHeight;
	depthStencilConfig.allowGPUWrite = true;
	depthStencilConfig.allowCPURead = false;
	depthStencilConfig.allowCPUWrite = false;
	depthStencilConfig.mipLevels = 1;
	depthStencilConfig.arraySize = 1;
	depthStencilConfig.pixelFormat = FORMAT_D32_FLOAT;
	depthStencilConfig.purposeFlags = Texture::Purpose::DEPTH_STENCIL;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	checkDirectXCall(swapChain_->GetDesc(&swapChainDesc), "Failed to retrieve the swap chain description");
	depthStencilConfig.sampleCount = swapChainDesc.SampleDesc.Count;
	depthStencilConfig.sampleQuality = swapChainDesc.SampleDesc.Quality;

	depthStencil_ = Texture(*this, depthStencilConfig);
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
