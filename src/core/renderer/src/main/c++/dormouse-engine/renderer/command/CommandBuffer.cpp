#include "CommandBuffer.hpp"

#include <algorithm>

using namespace dormouse_engine::renderer::command;

void CommandBuffer::submit(dormouse_engine::graphics::CommandList& commandList) {
	std::sort(index_.begin(), index_.end(), [](const auto lhs, const auto rhs) {
			return std::get<CommandKey>(lhs).hash < std::get<CommandKey>(rhs).hash;
		});

	auto* previousCommand = static_cast<Command*>(nullptr);

	for (const auto& entry : index_) {
		const auto commandIndex = std::get<size_t>(entry);

		commands_[commandIndex].submit(commandList, previousCommand);
		previousCommand = &commands_[commandIndex];
	}

	commands_.clear();
}
