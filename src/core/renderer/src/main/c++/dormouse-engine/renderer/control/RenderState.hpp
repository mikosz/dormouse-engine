#ifndef _DORMOUSEENGINE_RENDERER_CONTROL_RENDERSTATE_HPP_
#define _DORMOUSEENGINE_RENDERER_CONTROL_RENDERSTATE_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/RenderState.hpp"
#include "dormouse-engine/graphics/CommandList.hpp"

namespace dormouse_engine::renderer::control {

class RenderState final {
public:

	static constexpr graphics::RenderState::Configuration OPAQUE = {
		graphics::RenderState::CullMode::BACK,
		graphics::RenderState::FillMode::SOLID,
		false,
		false
		};

	RenderState(graphics::Device& graphicsDevice, graphics::RenderState::Configuration configuration);

	void bind(graphics::CommandList& commandList) const;

private:

	size_t renderStateId_;

	friend bool operator==(const RenderState& lhs, const RenderState& rhs) {
		return lhs.renderStateId_ == rhs.renderStateId_;
	}

	friend bool operator!=(const RenderState& lhs, const RenderState& rhs) {
		return !(lhs == rhs);
	}

};

} // namespace dormouse_engine::renderer::control

#endif /* _DORMOUSEENGINE_RENDERER_CONTROL_RENDERSTATE_HPP_ */
