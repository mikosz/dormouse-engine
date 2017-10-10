#include "CommandBuffer.hpp"

#include <algorithm>

using namespace dormouse_engine::renderer::command;

void CommandBuffer::submit(dormouse_engine::graphics::CommandList& commandList) {
	std::sort(commands_.begin(), commands_.end(), [](const auto lhs, const auto rhs) {
			return lhs->key().hash < rhs->key().hash;
		});

	const auto* previousCommand = static_cast<const Command*>(nullptr);

	for (const auto& command : commands_) {
		command->submit(commandList, previousCommand);
		previousCommand = command.get();
	}

	commands_.clear();
}
