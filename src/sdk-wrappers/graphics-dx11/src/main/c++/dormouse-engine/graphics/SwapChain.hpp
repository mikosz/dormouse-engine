#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_SWAPCHAIN_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_SWAPCHAIN_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"

namespace dormouse_engine {
namespace graphics {

class Device;

class SwapChain {
public:

	SwapChain(Device& device);

private:

	system::windows::COMWrapper<IDXGISwapChain> swapChain_;

};

} // namespace graphics
} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_SWAPCHAIN_HPP_ */
