#ifndef _DORMOUSEENGINE_RENDERER_RENDERER_HPP_
#define _DORMOUSEENGINE_RENDERER_RENDERER_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Texture.hpp"

namespace dormouse_engine::renderer {

class Renderer {
public:

	Renderer(graphics::Device& graphicsDevice) :
		renderTarget_(graphicsDevice.backBuffer()),
		depthStencil_(graphicsDevice.depthStencil())
	{
	}

	void beginScene();

	void endScene();

private:

	graphics::RenderTargetView renderTarget_;

	graphics::DepthStencilView depthStencil_;

};

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_RENDERER_HPP_ */
