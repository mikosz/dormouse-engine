#include "Renderer.hpp"

using namespace dormouse_engine::renderer;

void Renderer::beginScene() {
	// TODO: move to pulp or disperse
	float colour[] = { 1.0f, 0.0f, 1.0f, 1.0f };

	auto& dxDeviceContext = detail::Internals::dxDeviceContext(immediateCommandList_);

	dxDeviceContext.ClearRenderTargetView(&detail::Internals::dxRenderTargetView(backBuffer_), colour);
	dxDeviceContext.ClearDepthStencilView(
		detail::Internals::dxDepthStencilView(depthStencil_),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		static_cast<UINT8>(0)
		);
}

void Renderer::endScene() {
	swapChain_->Present(configuration_.vsync, 0);
}
