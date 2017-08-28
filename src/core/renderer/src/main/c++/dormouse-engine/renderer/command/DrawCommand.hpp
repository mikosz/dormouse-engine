#ifndef _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_

#include "dormouse-engine/graphics/CommandList.hpp"
#include "dormouse-engine/graphics/VertexBuffer.hpp"
#include "dormouse-engine/graphics/IndexBuffer.hpp"
#include "dormouse-engine/graphics/PrimitiveTopology.hpp"
#include "../control/RenderState.hpp"
#include "../control/Sampler.hpp"
#include "CommandKey.hpp"

namespace dormouse_engine::renderer::command {

class DrawCommand final {
public:

	CommandKey key() const {
		return key_;
	}

	void submit(graphics::CommandList& commandList, const DrawCommand* previous) const;

	void setRenderState(control::RenderState renderState) {
		renderState_ = std::move(renderState);
	}

	void setSampler(control::Sampler sampler, graphics::ShaderType stage, size_t slot) {
		set sampler at stage & slot to sampler (want an array)
	}

	void setVertexBuffer(graphics::VertexBuffer vertexBuffer) {
		vertexBuffer_ = std::move(vertexBuffer);
	}

	void setIndexBuffer(graphics::IndexBuffer indexBuffer, size_t indexCount) {
		indexBuffer_ = std::move(indexBuffer);
		indexCount_ = indexCount;
	}

private:

	CommandKey key_;

	control::RenderState renderState_;

	...control::Sampler samplers_;

	graphics::VertexBuffer vertexBuffer_;

	graphics::IndexBuffer indexBuffer_;

	size_t indexCount_;

	graphics::PrimitiveTopology primitiveTopology_;

};

} // namespace dormouse_engine::renderer::command

#endif /* _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_ */
