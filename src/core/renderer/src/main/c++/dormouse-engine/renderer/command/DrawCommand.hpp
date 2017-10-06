#ifndef _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_

#pragma warning(push, 3)
#	include <ponder/pondertype.hpp>
#pragma warning(pop)

#include "dormouse-engine/graphics/CommandList.hpp"
#include "dormouse-engine/graphics/Buffer.hpp"
#include "dormouse-engine/graphics/PrimitiveTopology.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/essentials/memory.hpp"
#include "../control/RenderState.hpp"
#include "../control/Sampler.hpp"
#include "../control/ResourceView.hpp"
#include "../control/RenderTargetView.hpp"
#include "../control/DepthStencilView.hpp"
#include "../control/Viewport.hpp"
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

	void setViewport(control::Viewport viewport) {
		viewport_ = std::move(viewport);
	}

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

	void setVertexBuffer(graphics::Buffer vertexBuffer, size_t vertexCount, size_t vertexStride) {
		vertexBuffer_ = std::move(vertexBuffer);
		vertexCount_ = vertexCount;
		vertexStride_ = vertexStride;
	}

	void setIndexBuffer(graphics::Buffer indexBuffer, size_t indexCount) {
		indexBuffer_ = std::move(indexBuffer);
		indexCount_ = indexCount;
	}

	void setPrimitiveTopology(graphics::PrimitiveTopology primitiveTopology) {
		primitiveTopology_ = primitiveTopology;
	}

	void setConstantBuffer(graphics::Buffer buffer, graphics::ShaderType stage, size_t slot) {
		constantBuffer_(stage, slot) = std::move(buffer);
	}

	essentials::ConstBufferView constantBufferData(graphics::ShaderType stage, size_t slot) const {
		assert(static_cast<size_t>(stage) < STAGE_COUNT);
		assert(slot < graphics::CONSTANT_BUFFER_SLOT_COUNT_PER_SHADER);
		const auto idx = static_cast<size_t>(stage) * graphics::CONSTANT_BUFFER_SLOT_COUNT_PER_SHADER + slot;
		return essentials::viewBuffer(constantBufferData_[idx]);
	}

	essentials::ByteVector& constantBufferData(graphics::ShaderType stage, size_t slot) {
		assert(static_cast<size_t>(stage) < STAGE_COUNT);
		assert(slot < graphics::CONSTANT_BUFFER_SLOT_COUNT_PER_SHADER);
		const auto idx = static_cast<size_t>(stage) * graphics::CONSTANT_BUFFER_SLOT_COUNT_PER_SHADER + slot;
		return constantBufferData_[idx];
	}

private:

	static_assert(static_cast<size_t>(graphics::ShaderType::VERTEX) == 0u);
	static_assert(static_cast<size_t>(graphics::ShaderType::GEOMETRY) == 1u);
	static_assert(static_cast<size_t>(graphics::ShaderType::HULL) == 2u);
	static_assert(static_cast<size_t>(graphics::ShaderType::DOMAIN) == 3u);
	static_assert(static_cast<size_t>(graphics::ShaderType::PIXEL) == 4u);

	static constexpr auto STAGE_COUNT = 5u; // vs, gs, hs, ds, ps

	CommandKey key_;

	control::Viewport viewport_;

	control::RenderTargetView renderTarget_;

	control::DepthStencilView depthStencil_;

	essentials::observer_ptr<const shader::Technique> technique_;

	control::RenderState renderState_;

	// TODO: consider a different way of storing samplers, resources etc. Arrays are great, because they
	// store data locally, but most of these control objects are null, so they're a waste of space.
	std::array<control::Sampler, STAGE_COUNT * graphics::SAMPLER_SLOT_COUNT_PER_SHADER> samplers_;

	std::array<control::ResourceView, STAGE_COUNT * graphics::RESOURCE_SLOT_COUNT_PER_SHADER> resources_;

	// TODO: keeping graphics::Buffer here disallows updating constant buffer data only when it changes,
	// i.e. if you have a constant buffer keeping per-frame data and you render multiple objects with it
	// it will always be updated. Create control::Buffer that's capable of checking whether buffer data
	// has changed and ensure that it is shared between objects by other moving it to shader::Shader or
	// by allowing some extra kind of identification.
	std::array<graphics::Buffer, STAGE_COUNT * graphics::CONSTANT_BUFFER_SLOT_COUNT_PER_SHADER> constantBuffers_;

	std::array<essentials::ByteVector, STAGE_COUNT * graphics::CONSTANT_BUFFER_SLOT_COUNT_PER_SHADER> constantBufferData_;

	graphics::Buffer vertexBuffer_;

	size_t vertexCount_ = 0u;

	size_t vertexStride_ = 0u;

	graphics::Buffer indexBuffer_;

	size_t indexCount_ = 0u;

	graphics::PrimitiveTopology primitiveTopology_;

	const control::Sampler& sampler_(graphics::ShaderType stage, size_t slot) const {
		assert(static_cast<size_t>(stage) < STAGE_COUNT);
		assert(slot < graphics::SAMPLER_SLOT_COUNT_PER_SHADER);
		return samplers_[static_cast<size_t>(stage) * graphics::SAMPLER_SLOT_COUNT_PER_SHADER + slot];
	}

	control::Sampler& sampler_(graphics::ShaderType stage, size_t slot) {
		return const_cast<control::Sampler&>(const_cast<const DrawCommand&>(*this).sampler_(stage, slot));
	}

	const control::ResourceView& resource_(graphics::ShaderType stage, size_t slot) const {
		assert(static_cast<size_t>(stage) < STAGE_COUNT);
		assert(slot < graphics::RESOURCE_SLOT_COUNT_PER_SHADER);
		return resources_[static_cast<size_t>(stage) * graphics::RESOURCE_SLOT_COUNT_PER_SHADER + slot];
	}

	control::ResourceView& resource_(graphics::ShaderType stage, size_t slot) {
		return const_cast<control::ResourceView&>(const_cast<const DrawCommand&>(*this).resource_(stage, slot));
	}

	const graphics::Buffer& constantBuffer_(graphics::ShaderType stage, size_t slot) const {
		assert(static_cast<size_t>(stage) < STAGE_COUNT);
		assert(slot < graphics::CONSTANT_BUFFER_SLOT_COUNT_PER_SHADER);
		return constantBuffers_[static_cast<size_t>(stage) * graphics::CONSTANT_BUFFER_SLOT_COUNT_PER_SHADER + slot];
	}

	graphics::Buffer& constantBuffer_(graphics::ShaderType stage, size_t slot) {
		return const_cast<graphics::Buffer&>(const_cast<const DrawCommand&>(*this).constantBuffer_(stage, slot));
	}

};

namespace detail { void declareDrawCommand(); }

} // namespace dormouse_engine::renderer::command

PONDER_AUTO_TYPE(
	dormouse_engine::renderer::command::DrawCommand,
	&dormouse_engine::renderer::command::detail::declareDrawCommand
);

#endif /* _DORMOUSEENGINE_RENDERER_COMMAND_DRAWCOMMAND_HPP_ */
