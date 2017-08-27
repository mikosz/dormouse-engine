#include "DrawCommand.hpp"

#include <cassert>

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::command;

void DrawCommand::submit(graphics::CommandList& commandList, const DrawCommand* previous) const {
	if (!previous || previous->renderState_ != renderState_) {
		commandList.setRenderState(renderState_);
	}

	commandList.setVertexBuffer(vertexBuffer_, 0u);
	commandList.setIndexBuffer(indexBuffer_, 0u);

	commandList.drawIndexed();
}
