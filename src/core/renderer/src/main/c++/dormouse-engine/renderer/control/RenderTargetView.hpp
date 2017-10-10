#ifndef _DORMOUSEENGINE_RENDERER_CONTROL_RENDERTARGETVIEW_HPP_
#define _DORMOUSEENGINE_RENDERER_CONTROL_RENDERTARGETVIEW_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Texture.hpp"
#include "dormouse-engine/graphics/CommandList.hpp"

namespace dormouse_engine::renderer::control {

class RenderTargetView final {
public:

	static void initialiseSystem(graphics::Device& graphicsDevice);

	RenderTargetView() = default;

	RenderTargetView(const graphics::Texture& texture);

	graphics::RenderTargetView get() const;

private:

	static constexpr size_t INVALID_RENDERTARGET_VIEW_ID = static_cast<size_t>(-1);

	size_t renderTargetViewId_ = INVALID_RENDERTARGET_VIEW_ID;

	friend bool operator==(const RenderTargetView& lhs, const RenderTargetView& rhs) {
		return lhs.renderTargetViewId_ == rhs.renderTargetViewId_;
	}

	friend bool operator!=(const RenderTargetView& lhs, const RenderTargetView& rhs) {
		return !(lhs == rhs);
	}

};

} // namespace dormouse_engine::renderer::control

#endif /* _DORMOUSEENGINE_RENDERER_CONTROL_RENDERTARGETVIEW_HPP_ */
