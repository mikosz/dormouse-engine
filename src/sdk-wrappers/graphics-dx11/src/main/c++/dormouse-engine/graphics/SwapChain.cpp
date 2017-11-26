#include "graphics.pch.hpp"

#include "SwapChain.hpp"

#include "detail/Internals.hpp"
#include "DirectXError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

namespace /* anonymous */
{

system::windows::COMWrapper<IDXGISwapChain> createSwapChain(
	Device& device, wm::Window& window, const SwapChain::Configuration& configuration)
{
	auto swapChain = system::windows::COMWrapper<IDXGISwapChain>();

	auto refreshRate = DXGI_RATIONAL();
	refreshRate.Numerator = static_cast<UINT>(configuration.displayMode.refreshRateNumerator);
	refreshRate.Denominator = static_cast<UINT>(configuration.displayMode.refreshRateDenominator);

	auto swapChainDesc = DXGI_SWAP_CHAIN_DESC();
	std::memset(&swapChainDesc, 0, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = static_cast<UINT>(configuration.displayMode.width);
	swapChainDesc.BufferDesc.Height = static_cast<UINT>(configuration.displayMode.height);
	swapChainDesc.BufferDesc.Format = static_cast<DXGI_FORMAT>(configuration.displayMode.pixelFormat.id());
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	if (configuration.vsync) {
		swapChainDesc.BufferDesc.RefreshRate = refreshRate;
	} else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	auto msaaSampleCount =
		std::min<UINT>(D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT, static_cast<UINT>(configuration.msaaSampleCount));
	auto msaaSampleQuality = UINT();

	auto& dxDevice = detail::Internals::dxDevice(device);

	for (;;) {
		if (msaaSampleCount == 1) {
			msaaSampleQuality = 0;
			break;
		}

		auto maxSampleQuality = UINT();
		checkDirectXCall(
			dxDevice.CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, msaaSampleCount, &maxSampleQuality),
			"Failed to retrieve the multisampling quality level"
			);
		if (maxSampleQuality == 0) {
			msaaSampleCount /= 2;
		} else {
			msaaSampleQuality = std::min(static_cast<UINT>(configuration.msaaSampleQuality), maxSampleQuality - 1);
			break;
		}
	}

	swapChainDesc.SampleDesc.Count = msaaSampleCount;
	swapChainDesc.SampleDesc.Quality = msaaSampleQuality;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window.handle();
	swapChainDesc.Windowed = !configuration.fullscreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	checkDirectXCall(
		detail::Internals::createDXGIFactory()->CreateSwapChain(
			&dxDevice,
			&swapChainDesc,
			&swapChain.get()
			),
		"Failed to create a directx swap chain"
		);

	return swapChain;
}

Texture extractBackBuffer(IDXGISwapChain& swapChain) {
	auto texture = system::windows::COMWrapper<ID3D11Texture2D>();

	checkDirectXCall(
		swapChain.GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture.get())),
		"Failed to extract the back buffer texture"
		);

	return detail::Internals::createTextureFromDX11Texture(std::move(texture));
}

} // anonymous namespace

SwapChain::SwapChain(Device& device, wm::Window& window, const Configuration& configuration) :
	swapChain_(createSwapChain(device, window, configuration)),
	backBuffer_(extractBackBuffer(*swapChain_)),
	backBufferRTV_(backBuffer_)
{
}

SwapChain::SwapChain(Texture backBufferTexture) :
	backBuffer_(std::move(backBufferTexture)),
	backBufferRTV_(backBuffer_)
{
}

void SwapChain::clear() {
	auto& device = detail::Internals::dxDevice(backBuffer_);
	auto deviceContext = system::windows::COMWrapper<ID3D11DeviceContext>();
	device.GetImmediateContext(&deviceContext.get());

	float clearColour[] = { 1.0f, 0.0f, 1.0f, 1.0f };

	deviceContext->ClearRenderTargetView(&detail::Internals::dxRenderTargetView(backBufferRTV_), clearColour);
}

void SwapChain::present() const {
	if (swapChain_) {
		checkDirectXCall(
			swapChain_->Present(vsync_ ? 1u : 0u, 0u),
			"Swap chain present failed"
			);
	}
}
