#ifndef _DORMOUSEENGINE_RENDERER_RENDERER_HPP_
#define _DORMOUSEENGINE_RENDERER_RENDERER_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Texture.hpp"
#include "dormouse-engine/graphics/SwapChain.hpp"

namespace dormouse_engine::renderer {

class Renderer {
public:

	Renderer(graphics::Device& graphicsDevice) :
		backBuffer_(graphicsDevice.backBuffer()),
		depthBuffer_(graphicsDevice.depthStencil()),
		swapChain_(graphicsDevice)
	{
	}

	void beginScene();

	void endScene();

private:

	graphics::RenderTargetView backBuffer_;

	graphics::DepthStencilView depthBuffer_;

	graphics::SwapChain swapChain_;

};

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_RENDERER_HPP_ */
