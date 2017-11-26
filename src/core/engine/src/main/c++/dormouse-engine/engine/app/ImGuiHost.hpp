#ifndef _DORMOUSEENGINE_ENGINE_APP_IMGUIHOST_HPP_
#define _DORMOUSEENGINE_ENGINE_APP_IMGUIHOST_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Buffer.hpp"
#include "dormouse-engine/renderer/shader/Technique.hpp"
#include "dormouse-engine/renderer/control/ResourceView.hpp"
#include "dormouse-engine/renderer/control/Control.hpp"
#include "dormouse-engine/renderer/command/commandfwd.hpp"
#include "dormouse-engine/renderer/command/DrawCommand.hpp"
#include "../time/Timer.hpp"

struct ImDrawData;

namespace dormouse_engine::engine::app {

class ImGuiHost final {
public:

	ImGuiHost(
		time::Timer timer,
		graphics::Device& graphicsDevice,
		graphics::Texture renderTarget,
		size_t width,
		size_t height
		);

	void update();

	void render(
		graphics::Device& graphicsDevice, renderer::command::CommandBuffer& rendererCommandBuffer);

private:

	renderer::control::ResourceView imguiFontAtlas_;

	time::Timer timer_;

	graphics::Buffer vertexBuffer_;

	size_t vertexBufferSize_ = 0;

	size_t vertexCount_ = 0;

	graphics::Buffer indexBuffer_;

	size_t indexBufferSize_ = 0;

	size_t indexCount_ = 0;

	graphics::Buffer constantBuffer_;

	renderer::shader::Technique technique_;

	renderer::control::Control renderControl_;

	void populateBuffers_(graphics::Device& graphicsDevice, const ImDrawData& imguiDrawData);

};

} // namespace dormouse_engine::engine::app

#endif /* _DORMOUSEENGINE_ENGINE_APP_IMGUIHOST_HPP_ */
