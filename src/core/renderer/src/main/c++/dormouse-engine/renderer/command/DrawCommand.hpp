#ifndef _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_

#include "dormouse-engine/graphics/CommandList.hpp"
#include "dormouse-engine/graphics/Buffer.hpp"
#include "dormouse-engine/graphics/PrimitiveTopology.hpp"
#include "../control/RenderState.hpp"
#include "../control/Sampler.hpp"
#include "../control/ResourceView.hpp"
#include "../shader/Technique.hpp"
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
		sampler_(stage, slot) = std::move(sampler);
	}

	void setResource(control::ResourceView resource, graphics::ShaderType stage, size_t slot) {
		resource_(stage, slot) = std::move(resource);
	}

	void setTechnique(shader::Technique technique) {
		technique_ = std::move(technique);
	}

	void setVertexBuffer(graphics::Buffer vertexBuffer) {
		vertexBuffer_ = std::move(vertexBuffer);
	}

	void setIndexBuffer(graphics::Buffer indexBuffer, size_t indexCount) {
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

	std::array<control::ResourceView, STAGE_COUNT * graphics::RESOURCE_SLOT_COUNT_PER_SHADER> resources_;

	shader::Technique technique_;

	graphics::Buffer vertexBuffer_;

	graphics::Buffer indexBuffer_;

	size_t indexCount_;

	graphics::PrimitiveTopology primitiveTopology_;

	control::Sampler& sampler_(graphics::ShaderType stage, size_t slot) {
		assert(static_cast<size_t>(stage) < 5u);
		assert(slot < graphics::SAMPLER_SLOT_COUNT_PER_SHADER);
		return samplers_[static_cast<size_t>(stage) * graphics::SAMPLER_SLOT_COUNT_PER_SHADER + slot];
	}

	control::Sampler sampler_(graphics::ShaderType stage, size_t slot) const {
		return const_cast<DrawCommand&>(*this).sampler_(stage, slot);
	}

	control::ResourceView resource_(graphics::ShaderType stage, size_t slot) {
		assert(static_cast<size_t>(stage) < 5u);
		assert(slot < graphics::RESOURCE_SLOT_COUNT_PER_SHADER);
		return resources_[static_cast<size_t>(stage) * graphics::RESOURCE_SLOT_COUNT_PER_SHADER + slot];
	}

	control::ResourceView resource_(graphics::ShaderType stage, size_t slot) const {
		return const_cast<DrawCommand&>(*this).resource_(stage, slot);
	}

};

} // namespace dormouse_engine::renderer::command

#endif /* _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_ */
