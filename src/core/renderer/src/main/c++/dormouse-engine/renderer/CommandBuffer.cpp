#include "CommandBuffer.hpp"

#include <algorithm>

using namespace dormouse_engine::renderer;

void CommandBuffer::submit(dormouse_engine::graphics::CommandList& commandList) {
	std::sort(index_.begin(), index_.end(), [](const auto lhs, const auto rhs) {
			return std::get<CommandKey>(lhs).hash < std::get<CommandKey>(rhs).hash;
		});

	constexpr auto INVALID_INDEX = static_cast<size_t>(-1);
	auto previous = INVALID_INDEX;

	for (const auto& entry : index_) {
		const auto commandIndex = std::get<size_t>(entry);

		const auto* previousCommand = static_cast<DrawCommand*>(nullptr);
		if (previous != INVALID_INDEX) {
			previousCommand = &commands_[previous];
		}

		commands_[commandIndex].submit(commandList, previousCommand);
	}
}
