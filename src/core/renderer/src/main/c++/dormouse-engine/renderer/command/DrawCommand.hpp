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

	// TODO: graphics::shadertype is called stage here. Should rename graphics::shader to graphics::shaderstage
	// which would avoid having two names denoting different things
	void setSampler(control::Sampler sampler, graphics::ShaderType stage, size_t slot) {		
		sampler_(stage, slot) = std::move(sampler);
	}

	void setVertexBuffer(graphics::VertexBuffer vertexBuffer) {
		vertexBuffer_ = std::move(vertexBuffer);
	}

	void setIndexBuffer(graphics::IndexBuffer indexBuffer, size_t indexCount) {
		indexBuffer_ = std::move(indexBuffer);
		indexCount_ = indexCount;
	}

private:

	static_assert(static_cast<size_t>(graphics::ShaderType::VERTEX) == 0u);
	static_assert(static_cast<size_t>(graphics::ShaderType::GEOMETRY) == 1u);
	static_assert(static_cast<size_t>(graphics::ShaderType::HULL) == 2u);
	static_assert(static_cast<size_t>(graphics::ShaderType::DOMAIN) == 3u);
	static_assert(static_cast<size_t>(graphics::ShaderType::PIXEL) == 4u);

	static constexpr auto STAGE_COUNT = 5u; // vs, gs, hs, ds, ps

	CommandKey key_;

	control::RenderState renderState_;

	std::array<control::Sampler, STAGE_COUNT * graphics::SAMPLER_SLOT_COUNT_PER_SHADER> samplers_;

	graphics::VertexBuffer vertexBuffer_;

	graphics::IndexBuffer indexBuffer_;

	size_t indexCount_;

	graphics::PrimitiveTopology primitiveTopology_;

	control::Sampler& sampler_(graphics::ShaderType stage, size_t slot) {
		assert(static_cast<size_t>(stage) < 5u);
		assert(slot < STAGE_COUNT);
		return samplers_[static_cast<size_t>(stage) * graphics::SAMPLER_SLOT_COUNT_PER_SHADER + slot];
	}

	control::Sampler sampler_(graphics::ShaderType stage, size_t slot) const {
		return const_cast<DrawCommand&>(*this).sampler_(stage, slot);
	}

};

} // namespace dormouse_engine::renderer::command

#endif /* _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_ */
