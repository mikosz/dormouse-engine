#ifndef _DORMOUSEENGINE_RENDERER_D2_SPRITE_HPP_
#define _DORMOUSEENGINE_RENDERER_D2_SPRITE_HPP_

#pragma warning(push, 3)
#	include <ponder/pondertype.hpp>
#pragma warning(pop)

#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Texture.hpp"
#include "dormouse-engine/math/Transform.hpp"
#include "../control/ResourceView.hpp"
#include "../control/controlfwd.hpp"
#include "../command/commandfwd.hpp"
#include "../shader/Property.hpp"
#include "Layout.hpp"

namespace dormouse_engine::renderer::d2 {

class Sprite final {
public:

	// TODO: temp - don't pass shader code here
	static void initialiseSystem(graphics::Device& device, essentials::ConstBufferView shaderCode);

	Sprite(const graphics::Texture& texture) :
		textureView_(texture)
	{
	}

	void render(
		command::CommandBuffer& commandBuffer,
		const shader::Property& properties,
		const control::Control& renderControl
		) const;

	control::ResourceView texture() const noexcept {
		return textureView_;
	}

	control::Sampler sampler() const noexcept;

	const Layout& layout() const noexcept {
		return layout_;
	}

	Layout& layout() noexcept {
		return layout_;
	}

private:

	control::ResourceView textureView_;

	Layout layout_;

};

namespace detail { void declareSprite(); }

} // namespace dormouse_engine::renderer::d2

PONDER_AUTO_TYPE(dormouse_engine::renderer::d2::Sprite, &dormouse_engine::renderer::d2::detail::declareSprite);

#endif /* _DORMOUSEENGINE_RENDERER_D2_SPRITE_HPP_ */
