#include "graphics.pch.hpp"

#include "BackBuffer.hpp"

#include <cassert>
#include <algorithm>

#include "DirectXError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

namespace /* anonymous */ {



} // anonymous namespace

BackBuffer::BackBuffer(Device& device)
{
	auto screenWidth = size_t();
	auto screenHeight = size_t();
	std::tie(screenWidth, screenHeight) = windowDimensions(windowHandle);

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
