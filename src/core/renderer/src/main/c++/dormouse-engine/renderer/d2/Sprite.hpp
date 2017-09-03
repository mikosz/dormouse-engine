#ifndef _DORMOUSEENGINE_RENDERER_D2_SPRITE_HPP_
#define _DORMOUSEENGINE_RENDERER_D2_SPRITE_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Texture.hpp"
#include "../control/ResourceView.hpp"
#include "../command/CommandBuffer.hpp"
#include "../command/DrawCommand.hpp"

namespace dormouse_engine::renderer::d2 {

class Sprite final {
public:

	static void initialiseSystem(graphics::Device& device);

	Sprite(const graphics::Texture& texture);

	void render(command::CommandBuffer& commandBuffer) const;

private:	

	mutable command::DrawCommand cmd_;

};

} // namespace dormouse_engine::renderer::d2

#endif /* _DORMOUSEENGINE_RENDERER_D2_SPRITE_HPP_ */
