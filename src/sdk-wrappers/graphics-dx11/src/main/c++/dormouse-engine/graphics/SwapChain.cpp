#include "SwapChain.hpp"

#include "detail/Internals.hpp"
#include "DirectXError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

namespace /* anonymous */
{

system::windows::COMWrapper<IDXGISwapChain> createSwapChain(Device& device, const SwapChain::Configuration& configuration) {
	auto swapChain = system::windows::COMWrapper<IDXGISwapChain>();

	auto refreshRate = DXGI_RATIONAL();
	refreshRate.Numerator = configuration.displayMode.refreshRateNumerator;
	refreshRate.Denominator = configuration.displayMode.refreshRateDenominator;

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

Texture extractBackBuffer(IDXGISwapChain& swapChain) {
	auto texture = system::windows::COMWrapper<ID3D11Texture2D>();

	checkDirectXCall(
		swapChain.GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture.get())),
		"Failed to extract the back buffer texture"
		);

	return detail::Internals::createTextureFromDX11Texture(std::move(texture));
}

} // anonymous namespace

SwapChain::SwapChain(Device& device, const SwapChain::Configuration& configuration) :
	swapChain_(createSwapChain(device, configuration)),
	backBuffer_(extractBackBuffer(*swapChain_))
{
}
