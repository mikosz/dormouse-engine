#include "graphics.pch.hpp"

#include "BackBuffer.hpp"

#include <cassert>
#include <algorithm>

#include "DirectXError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

namespace /* anonymous */ {

void queryAdapterAndRefreshRate(
	IDXGIFactory& dxgiFactory,
	system::windows::COMWrapper<IDXGIAdapter>* adapter
	)
{
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

Texture extractBackBuffer(IDXGISwapChain* swapChain) {
	auto texture = system::windows::COMWrapper<ID3D11Texture2D>();

	checkDirectXCall(
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture.get())),
		"Failed to extract the back buffer texture"
	);

	return detail::Internals::createTextureFromDX11Texture(std::move(texture));
}

system::windows::COMWrapper<IDXGISwapChain> createSwapChain(Device& device) {
	assert(modeCount == displayModes.size());

	refreshRate->Numerator = 0;
	refreshRate->Denominator = 0;

	for (const auto& displayMode : displayModes) {
		if (displayMode.Width == screenWidth && displayMode.Height == screenHeight) {
			*refreshRate = displayMode.RefreshRate;
		}
	}

	auto swapChain = system::windows::COMWrapper<IDXGISwapChain>();

	auto swapChainDesc = DXGI_SWAP_CHAIN_DESC();
	std::memset(&swapChainDesc, 0, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;

	auto screenWidth = size_t();
	auto screenHeight = size_t();
	std::tie(screenWidth, screenHeight) = windowDimensions(windowHandle);

	swapChainDesc.BufferDesc.Width = static_cast<UINT>(screenWidth);
	swapChainDesc.BufferDesc.Height = static_cast<UINT>(screenHeight);
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // TODO: read from config or parameter
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	if (configuration.vsync) {
		swapChainDesc.BufferDesc.RefreshRate = refreshRate;
	} else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	UINT sampleCount = std::min<UINT>(D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT, configuration.sampleCount);
	UINT sampleQuality;

	for (;;) {
		if (sampleCount == 1) {
			sampleQuality = 0;
			break;
		}

		UINT maxSampleQuality;
		checkDirectXCall(
			(*device)->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &maxSampleQuality),
			"Failed to retrieve the multisampling quality level"
			);
		if (maxSampleQuality == 0) {
			sampleCount /= 2;
		} else {
			sampleQuality = std::min<UINT>(configuration.sampleQuality, maxSampleQuality - 1);
			break;
		}
	}

	swapChainDesc.SampleDesc.Count = sampleCount;
	swapChainDesc.SampleDesc.Quality = sampleQuality;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = windowHandle;
	swapChainDesc.Windowed = !configuration.fullscreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	checkDirectXCall(
		dxgiFactory.CreateSwapChain(
			*device,
			&swapChainDesc,
			&swapChain->get()
			),
		"Failed to create a directx swap chain"
		);

	return swapChain;
}

} // anonymous namespace

BackBuffer::BackBuffer(Device& device) :
	swapChain_(createSwapChain(device))
{
	auto screenWidth = size_t();
	auto screenHeight = size_t();
	std::tie(screenWidth, screenHeight) = windowDimensions(windowHandle);

	DXGI_RATIONAL refreshRate;
	queryAdapterAndRefreshRate(*dxgiFactory, &adapter_, &refreshRate, screenWidth, screenHeight);

	backBuffer_ = extractBackBuffer(swapChain_);

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

	UINT quality;
	d3dDevice_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quality); // TODO: literal in code

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	checkDirectXCall(swapChain_->GetDesc(&swapChainDesc), "Failed to retrieve the swap chain description");
	depthStencilConfig.sampleCount = swapChainDesc.SampleDesc.Count;
	depthStencilConfig.sampleQuality = swapChainDesc.SampleDesc.Quality;

	depthStencil_ = Texture(*this, depthStencilConfig);
}
