#ifndef _DORMOUSEENGINE_RENDERER_D2_SPRITE_HPP_
#define _DORMOUSEENGINE_RENDERER_D2_SPRITE_HPP_

#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Texture.hpp"
#include "dormouse-engine/math/Vector.hpp"
#include "../control/ResourceView.hpp"
#include "../control/RenderTargetView.hpp"
#include "../control/DepthStencilView.hpp"
#include "../command/CommandBuffer.hpp"
#include "../command/DrawCommand.hpp"
#include "../shader/Property.hpp"

namespace dormouse_engine::renderer::d2 {

class Sprite final {
public:

	static void initialiseSystem(graphics::Device& device, essentials::ConstBufferView shaderCode);

	Sprite(const graphics::Texture& texture) :
		textureView_(texture)
	{
	}

	void render(
		command::CommandBuffer& commandBuffer,
		const shader::Property& properties,
		control::RenderTargetView renderTarget,
		control::DepthStencilView depthStencil
		) const;

	control::ResourceView texture() const {
		return textureView_;
	}

private:	

	mutable command::DrawCommand cmd_;

	control::ResourceView textureView_;

	math::Vec2 position_;

};

bool hasShaderProperty(const Sprite& model, essentials::StringId id);
shader::Property getShaderProperty(const Sprite& model, essentials::StringId id);

} // namespace dormouse_engine::renderer::d2

#endif /* _DORMOUSEENGINE_RENDERER_D2_SPRITE_HPP_ */
