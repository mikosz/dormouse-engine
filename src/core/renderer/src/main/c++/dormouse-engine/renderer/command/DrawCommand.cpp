#include "DrawCommand.hpp"

#include <cassert>

#include "dormouse-engine/essentials/Range.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::command;

void DrawCommand::submit(graphics::CommandList& commandList, const Command* previous) const {
	const auto* previousCommand = dynamic_cast<const DrawCommand*>(previous);

	if (!previousCommand || (previousCommand->renderState_ != renderState_)) {
		renderState_.bind(commandList);
	}

	for (const auto stageIdx : range<size_t>(0u, STAGE_COUNT)) {
		for (const auto slotIdx : range<size_t>(0u, graphics::SAMPLER_SLOT_COUNT_PER_SHADER)) {
			const auto stage = static_cast<graphics::ShaderType>(stageIdx);
			const auto sampler = sampler_(stage, slotIdx);
			if (!previousCommand || (previousCommand->sampler_(stage, slotIdx) != sampler)) {
				sampler.bind(commandList, stage, slotIdx);
			}
		}
	}

	for (const auto stageIdx : range<size_t>(0u, STAGE_COUNT)) {
		for (const auto slotIdx : range<size_t>(0u, graphics::RESOURCE_SLOT_COUNT_PER_SHADER)) {
			const auto stage = static_cast<graphics::ShaderType>(stageIdx);
			const auto resource = resource_(stage, slotIdx);
			if (!previousCommand || (previousCommand->resource_(stage, slotIdx) != resource)) {
				resource.bind(commandList, stage, slotIdx);
			}
		}
	}

	assert(static_cast<bool>(technique_));
	technique_->bind(commandList);

	commandList.setVertexBuffer(vertexBuffer_, 0u);
	commandList.setIndexBuffer(indexBuffer_, 0u);

	if (indexCount_ > 0u) {
		commandList.drawIndexed(0u, indexCount_, primitiveTopology_);
	} else {
		commandList.draw(0u, vertexCount_, primitiveTopology_);
	}
}
