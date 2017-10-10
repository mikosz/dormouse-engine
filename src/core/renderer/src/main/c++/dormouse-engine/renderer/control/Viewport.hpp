#ifndef _DORMOUSEENGINE_RENDERER_CONTROL_VIEWPORT_HPP_
#define _DORMOUSEENGINE_RENDERER_CONTROL_VIEWPORT_HPP_

#include "dormouse-engine/graphics/Viewport.hpp"

namespace dormouse_engine::renderer::control {

class Viewport final {
public:

	Viewport() = default;

	Viewport(const graphics::Viewport::Configuration& configuration);

	graphics::Viewport get() const;

private:

	static constexpr size_t INVALID_VIEWPORT_OBJECT_ID = static_cast<size_t>(-1);

	size_t viewportObjectId_ = INVALID_VIEWPORT_OBJECT_ID;

	friend bool operator==(const Viewport& lhs, const Viewport& rhs) {
		return lhs.viewportObjectId_ == rhs.viewportObjectId_;
	}

	friend bool operator!=(const Viewport& lhs, const Viewport& rhs) {
		return !(lhs == rhs);
	}

};

} // namespace dormouse_engine::renderer::control

#endif /* _DORMOUSEENGINE_RENDERER_CONTROL_VIEWPORT_HPP_ */
