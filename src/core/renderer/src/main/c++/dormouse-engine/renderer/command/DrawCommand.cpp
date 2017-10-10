#include "DrawCommand.hpp"

#include <cassert>

#pragma warning(push, 3)
#	include <ponder/classbuilder.hpp>
#pragma warning(pop)

#include "dormouse-engine/essentials/Range.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::command;

void DrawCommand::submit(graphics::CommandList& commandList, const Command* previous) const {
	const auto* previousCommand = dynamic_cast<const DrawCommand*>(previous);

	if (!previousCommand || (previousCommand->control_.viewport() != control_.viewport())) {
		commandList.setViewport(control_.viewport().get());
	}

	if (!previousCommand ||
		(previousCommand->control_.renderTarget() != control_.renderTarget()) ||
		(previousCommand->control_.depthStencil() != control_.depthStencil())
		)
	{
		commandList.setRenderTarget(control_.renderTarget().get(), control_.depthStencil().get());
	}

	if (!previousCommand || (previousCommand->control_.renderState() != control_.renderState())) {
		commandList.setRenderState(control_.renderState().get());
	}

	for (const auto stageIdx : range<size_t>(0u, STAGE_COUNT)) {
		const auto stage = static_cast<graphics::ShaderType>(stageIdx);
		
		for (const auto samplerSlotIdx : range<size_t>(0u, graphics::SAMPLER_SLOT_COUNT_PER_SHADER)) {
			const auto sampler = sampler_(stage, samplerSlotIdx);
			if (!previousCommand || (previousCommand->sampler_(stage, samplerSlotIdx) != sampler)) {
				commandList.setSampler(sampler.get(), stage, samplerSlotIdx);
			}
		}

		for (const auto resourceSlotIdx : range<size_t>(0u, graphics::RESOURCE_SLOT_COUNT_PER_SHADER)) {
			const auto resource = resource_(stage, resourceSlotIdx);
			if (!previousCommand || (previousCommand->resource_(stage, resourceSlotIdx) != resource)) {
				commandList.setResource(resource.get(), stage, resourceSlotIdx);
			}
		}

		for (const auto constantBufferSlotIdx : range<size_t>(0u, graphics::CONSTANT_BUFFER_SLOT_COUNT_PER_SHADER)) {
			const auto constantBuffer = constantBuffer_(stage, constantBufferSlotIdx);
			const auto data = constantBufferData(stage, constantBufferSlotIdx);

			if (data.size() > 0u) {
				// TODO: lock should return a std::unique_ptr<BufferView, ...> instead of uint8_t. size is retrievable
				// from resource anyway
				auto outPtr = commandList.lock(constantBuffer, graphics::CommandList::LockPurpose::WRITE_DISCARD);
				std::memcpy(outPtr.pixels.get(), data.data(), data.size());
			}

			commandList.setConstantBuffer(constantBuffer, stage, constantBufferSlotIdx);
		}
	}

	assert(static_cast<bool>(technique_));
	technique_->bind(commandList);

	commandList.setVertexBuffer(vertexBuffer_, 0u, vertexStride_);
	commandList.setIndexBuffer(indexBuffer_, 0u);

	if (indexCount_ > 0u) {
		commandList.drawIndexed(0u, indexCount_, primitiveTopology_);
	} else {
		commandList.draw(0u, vertexCount_, primitiveTopology_);
	}
}

void detail::declareDrawCommand() {
	ponder::Class::declare<DrawCommand>("dormouse_engine::renderer::command::DrawCommand");
}
