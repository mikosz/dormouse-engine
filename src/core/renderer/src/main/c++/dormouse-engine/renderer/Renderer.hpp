#ifndef _DORMOUSEENGINE_RENDERER_RENDERER_HPP_
#define _DORMOUSEENGINE_RENDERER_RENDERER_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/SwapChain.hpp"

namespace dormouse_engine::renderer {

class Renderer {
public:

	Renderer(graphics::SwapChain swapChain);

private:

	graphics::SwapChain swapChain_;

};

} // dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_RENDERER_HPP_ */
