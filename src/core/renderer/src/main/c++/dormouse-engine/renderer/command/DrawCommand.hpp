#ifndef _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_

#include "dormouse-engine/graphics/CommandList.hpp"
#include "dormouse-engine/graphics/VertexBuffer.hpp"
#include "dormouse-engine/graphics/IndexBuffer.hpp"
#include "dormouse-engine/graphics/PrimitiveTopology.hpp"
#include "dormouse-engine/graphics/RenderState.hpp"
#include "CommandKey.hpp"

namespace dormouse_engine::renderer::command {

class DrawCommand final {
public:

	CommandKey key() const {
		return key_;
	}

	void submit(graphics::CommandList& commandList, const DrawCommand* previous) const;

	void setRenderState(graphics::RenderState renderState) {
		renderState_ = std::move(renderState);
	}

	void setVertexBuffer(graphics::VertexBuffer vertexBuffer) {
		vertexBuffer_ = std::move(vertexBuffer);
	}

	void setIndexBuffer(graphics::IndexBuffer indexBuffer) {
		indexBuffer_ = std::move(indexBuffer);
	}

private:

	CommandKey key_;

	graphics::RenderState renderState_;

	graphics::VertexBuffer vertexBuffer_;

	graphics::IndexBuffer indexBuffer_;

	graphics::PrimitiveTopology primitiveTopology_;

};

} // namespace dormouse_engine::renderer::command

#endif /* _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_ */
