#ifndef _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_

#include "dormouse-engine/graphics/CommandList.hpp"
#include "dormouse-engine/graphics/Buffer.hpp"
#include "dormouse-engine/graphics/PrimitiveTopology.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "../control/RenderState.hpp"
#include "../control/Sampler.hpp"
#include "../control/ResourceView.hpp"
#include "../control/RenderTargetView.hpp"
#include "../control/DepthStencilView.hpp"
#include "../shader/Technique.hpp"
#include "Command.hpp"
#include "CommandKey.hpp"

namespace dormouse_engine::renderer::command {

class DrawCommand final : public Command {
public:

	CommandKey key() const override {
		return key_;
	}

	void submit(graphics::CommandList& commandList, const Command* previous) const override;

	void setRenderTarget(control::RenderTargetView renderTarget) {
		renderTarget_ = std::move(renderTarget);
	}

	void setDepthStencil(control::DepthStencilView depthStencil) {
		depthStencil_ = std::move(depthStencil);
	}

	void setTechnique(essentials::observer_ptr<const shader::Technique> technique) {
		technique_ = std::move(technique);
	}

	void setRenderState(control::RenderState renderState) {
		renderState_ = std::move(renderState);
	}

	void setSampler(control::Sampler sampler, graphics::ShaderType stage, size_t slot) {		
		sampler_(stage, slot) = std::move(sampler);
	}

	void setResource(control::ResourceView resource, graphics::ShaderType stage, size_t slot) {
		resource_(stage, slot) = std::move(resource);
	}

	void setVertexBuffer(graphics::Buffer vertexBuffer, size_t vertexCount) {
		vertexBuffer_ = std::move(vertexBuffer);
		vertexCount_ = vertexCount;
	}

	void setIndexBuffer(graphics::Buffer indexBuffer, size_t indexCount) {
		indexBuffer_ = std::move(indexBuffer);
		indexCount_ = indexCount;
	}

	void setPrimitiveTopology(graphics::PrimitiveTopology primitiveTopology) {
		primitiveTopology_ = primitiveTopology;
	}

private:

	static_assert(static_cast<size_t>(graphics::ShaderType::VERTEX) == 0u);
	static_assert(static_cast<size_t>(graphics::ShaderType::GEOMETRY) == 1u);
	static_assert(static_cast<size_t>(graphics::ShaderType::HULL) == 2u);
	static_assert(static_cast<size_t>(graphics::ShaderType::DOMAIN) == 3u);
	static_assert(static_cast<size_t>(graphics::ShaderType::PIXEL) == 4u);

	static constexpr auto STAGE_COUNT = 5u; // vs, gs, hs, ds, ps

	CommandKey key_;

	control::RenderTargetView renderTarget_;

	control::DepthStencilView depthStencil_;

	essentials::observer_ptr<const shader::Technique> technique_;

	control::RenderState renderState_;

	std::array<control::Sampler, STAGE_COUNT * graphics::SAMPLER_SLOT_COUNT_PER_SHADER> samplers_;

	std::array<control::ResourceView, STAGE_COUNT * graphics::RESOURCE_SLOT_COUNT_PER_SHADER> resources_;

	graphics::Buffer vertexBuffer_;

	size_t vertexCount_ = 0u;

	graphics::Buffer indexBuffer_;

	size_t indexCount_ = 0u;

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
