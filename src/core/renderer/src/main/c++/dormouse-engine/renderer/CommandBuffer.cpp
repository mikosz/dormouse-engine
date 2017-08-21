#include "CommandBuffer.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine;
using namespace dormouse_engine::renderer;

void CommandBuffer::add(DrawCommandUniquePtr drawCommand) {
	commands_.emplace_back(std::move(drawCommand));
}

void CommandBuffer::submit(graphics::CommandList& commandList) {
	for (auto& command: commands_) { // TODO: obviously do sorting and stuff
		command->submit(commandList);
	}

	commands_.clear();
}
