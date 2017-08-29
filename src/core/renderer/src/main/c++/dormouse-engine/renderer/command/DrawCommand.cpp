#include "DrawCommand.hpp"

#include <cassert>

#include "dormouse-engine/essentials/Range.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::command;

void DrawCommand::submit(graphics::CommandList& commandList, const DrawCommand* previous) const {
	if (!previous || (previous->renderState_ != renderState_)) {
		renderState_.bind(commandList);
	}

	for (const auto stageIdx : range<size_t>(0u, STAGE_COUNT)) {
		for (const auto slotIdx : range<size_t>(0u, graphics::SAMPLER_SLOT_COUNT_PER_SHADER)) {
			const auto stage = static_cast<graphics::ShaderType>(stageIdx);
			const auto sampler = sampler_(stage, slotIdx);
			if (!previous || (previous->sampler_(stage, slotIdx) != sampler)) {
				sampler.bind(commandList, stage, slotIdx);
			}
		}
	}

	commandList.setVertexBuffer(vertexBuffer_, 0u);
	commandList.setIndexBuffer(indexBuffer_, 0u);

	commandList.drawIndexed(0u, indexCount_, primitiveTopology_);
}
