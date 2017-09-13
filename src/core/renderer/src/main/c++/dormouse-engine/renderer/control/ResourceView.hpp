#ifndef _DORMOUSEENGINE_RENDERER_CONTROL_RESOURCEVIEW_HPP_
#define _DORMOUSEENGINE_RENDERER_CONTROL_RESOURCEVIEW_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Buffer.hpp"
#include "dormouse-engine/graphics/Texture.hpp"
#include "dormouse-engine/graphics/CommandList.hpp"

namespace dormouse_engine::renderer::control {

class ResourceView final {
public:

	static void initialiseSystem(graphics::Device& graphicsDevice);

	ResourceView() = default;

	ResourceView(const graphics::Buffer& buffer, graphics::PixelFormat elementFormat);

	ResourceView(const graphics::Texture& texture);

	void bind(graphics::CommandList& commandList, graphics::ShaderType stage, size_t slot) const;

private:

	static constexpr size_t INVALID_RESOURCE_VIEW_ID = static_cast<size_t>(-1);

	size_t resourceViewId_ = INVALID_RESOURCE_VIEW_ID;

	friend bool operator==(const ResourceView& lhs, const ResourceView& rhs) {
		return lhs.resourceViewId_ == rhs.resourceViewId_;
	}

	friend bool operator!=(const ResourceView& lhs, const ResourceView& rhs) {
		return !(lhs == rhs);
	}

};

} // namespace dormouse_engine::renderer::control

#endif /* _DORMOUSEENGINE_RENDERER_CONTROL_RESOURCEVIEW_HPP_ */
