#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_SWAPCHAIN_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_SWAPCHAIN_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "BackBuffer.hpp"
#include "Adapter.hpp"

namespace dormouse_engine {
namespace graphics {

class Device;

class SwapChain {
public:

	struct Configuration {
		DisplayMode displayMode;
		bool fullscreen;
		bool vsync;
	};

	SwapChain() = default;

	SwapChain(Device& device, const Configuration& configuration);

private:

	system::windows::COMWrapper<IDXGISwapChain> swapChain_;

	BackBuffer backBuffer_;

};

} // namespace graphics
} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_SWAPCHAIN_HPP_ */
