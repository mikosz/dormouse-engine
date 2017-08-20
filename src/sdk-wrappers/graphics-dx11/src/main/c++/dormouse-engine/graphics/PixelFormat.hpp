#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_PIXELFORMAT_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_PIXELFORMAT_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/enums.hpp"

namespace dormouse_engine::graphics {

DE_ENUM_VALUES(
	PixelFormat,
	// colour formats
	(R32G32B32A32_FLOAT)(DXGI_FORMAT_R32G32B32A32_FLOAT)
	(R32G32B32_FLOAT)(DXGI_FORMAT_R32G32B32_FLOAT)
	(R32G32_FLOAT)(DXGI_FORMAT_R32G32_FLOAT)
	(R32_FLOAT)(DXGI_FORMAT_R32_FLOAT)

	(R8G8B8A8_UNORM_SRGB)(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)

	//(A8R8G8B8_UNORM)(DXGI_FORMAT_A8R8G8B8_UNORM)
	(R8G8B8A8_UNORM)(DXGI_FORMAT_R8G8B8A8_UNORM)
	(B8G8R8A8_UNORM)(DXGI_FORMAT_B8G8R8A8_UNORM)
	(B8G8R8X8_UNORM)(DXGI_FORMAT_B8G8R8X8_UNORM)

	(R32_UINT)(DXGI_FORMAT_R32_UINT)

	// depth formats
	(D32_FLOAT)(DXGI_FORMAT_D32_FLOAT)

	// index formats
	(I16_UINT)(DXGI_FORMAT_R16_UINT)
	(I32_UINT)(DXGI_FORMAT_R32_UINT)

	// compressed formats
	(BC1_UNORM)(DXGI_FORMAT_BC1_UNORM)
);

// TODO: refactor me, please
size_t formatSize(PixelFormat format);
size_t formatRowPitch(PixelFormat format, size_t width);
size_t formatSlicePitch(PixelFormat format, size_t height, size_t rowPitch);
const char* const formatHLSLType(PixelFormat format);

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_PIXELFORMAT_HPP_ */
