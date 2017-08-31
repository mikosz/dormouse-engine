#ifndef _DORMOUSEENGINE_RENDERER_DRAW_SPRITE_HPP_
#define _DORMOUSEENGINE_RENDERER_DRAW_SPRITE_HPP_

#include "dormouse-engine/graphics/Texture.hpp"
#include "../control/ResourceView.hpp"
#include "../command/CommandBuffer.hpp"

namespace dormouse_engine::renderer::draw {

class Sprite {
public:

	Sprite(const graphics::Texture& texture);

	void render(command::CommandBuffer& commandBuffer) const;

private:	

	control::ResourceView textureView_;

};

} // namespace dormouse_engine::renderer::draw

#endif /* _DORMOUSEENGINE_RENDERER_DRAW_SPRITE_HPP_ */
