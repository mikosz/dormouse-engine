#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_BACKBUFFER_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_BACKBUFFER_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "Texture.hpp"

namespace dormouse_engine {
namespace graphics {

class Device;

class BackBuffer {
public:

	BackBuffer() = default;

	BackBuffer(Texture colourBuffer, Texture depthBuffer) :
		colourBuffer_(std::move(colourBuffer)),
		depthBuffer_(std::move(depthBuffer))
	{
	}

private:

	Texture colourBuffer_;

	Texture depthBuffer_;

};

} // namespace graphics
} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_BACKBUFFER_HPP_ */
