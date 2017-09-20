#ifndef _DORMOUSEENGINE_RENDERER_CONTROL_VIEWPORT_HPP_
#define _DORMOUSEENGINE_RENDERER_CONTROL_VIEWPORT_HPP_

#include "dormouse-engine/graphics/Viewport.hpp"
#include "dormouse-engine/graphics/CommandList.hpp"

namespace dormouse_engine::renderer::control {

class Viewport final {
public:

	Viewport() = default;

	Viewport(const graphics::Viewport::Configuration& configuration);

	void bind(graphics::CommandList& commandList) const;

private:

	static constexpr size_t INVALID_VIEWPORT_ID = static_cast<size_t>(-1);

	size_t viewportId_ = INVALID_VIEWPORT_ID;

	friend bool operator==(const Viewport& lhs, const Viewport& rhs) {
		return lhs.viewportId_ == rhs.viewportId_;
	}

	friend bool operator!=(const Viewport& lhs, const Viewport& rhs) {
		return !(lhs == rhs);
	}

};

} // namespace dormouse_engine::renderer::control

#endif /* _DORMOUSEENGINE_RENDERER_CONTROL_VIEWPORT_HPP_ */
