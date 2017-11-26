#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_SWAPCHAIN_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_SWAPCHAIN_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "dormouse-engine/wm/Window.hpp"
#include "Adapter.hpp"
#include "Texture.hpp"

namespace dormouse_engine {
namespace graphics {

class Device;

class SwapChain {
public:

	struct Configuration {
		DisplayMode displayMode;
		bool fullscreen;
		bool vsync;
		size_t msaaSampleCount = 1;
		size_t msaaSampleQuality = 0;
	};

	SwapChain() = default;

	SwapChain(Device& device, wm::Window& window, const Configuration& configuration);

	SwapChain(Texture backBufferTexture);

	void clear();

	void present() const;

	Texture backBuffer() const noexcept {
		return backBuffer_;
	}

private:

	system::windows::COMWrapper<IDXGISwapChain> swapChain_;

	Texture backBuffer_;

	RenderTargetView backBufferRTV_;

	bool vsync_;

};

} // namespace graphics
} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_SWAPCHAIN_HPP_ */
