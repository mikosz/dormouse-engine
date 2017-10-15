#include "CommandBuffer.hpp"

#include <algorithm>

using namespace dormouse_engine::renderer::command;

DrawCommand& CommandBuffer::get(const CommandId& commandId) {
	auto it = drawCommandPoolIndex_.find(commandId);

	if (it != drawCommandPoolIndex_.end()) {
		it->second.lastFrameUsed = lastFrameIdx_ + 1;
		return drawCommandPool_[it->second.poolIndex];
	} else {
		// TODO: could be done much more efficiently, so this is temp, right?
		// TODO: also, potentially the pooling could be done by an external class for separation of concerns
		// especially with other caches coming in the future (e.g. filesystem)
		for (auto& indexEntry : drawCommandPoolIndex_) {
			if (indexEntry.second.lastFrameUsed < lastFrameIdx_) {
				const auto poolIndex = indexEntry.second.poolIndex;
				drawCommandPoolIndex_.erase(indexEntry.first);
				drawCommandPoolIndex_.emplace(
					commandId, DrawCommandPoolIndexEntry{ poolIndex, lastFrameIdx_ + 1 });
				return drawCommandPool_[poolIndex];
			}
		}

		drawCommandPool_.emplace_back();
		drawCommandPoolIndex_.emplace(
			commandId, DrawCommandPoolIndexEntry{ drawCommandPool_.size() - 1, lastFrameIdx_ + 1 });
		return drawCommandPool_.back();
	}
}

void CommandBuffer::submit(dormouse_engine::graphics::CommandList& commandList) {
	std::sort(commands_.begin(), commands_.end(), [](const auto lhs, const auto rhs) {
			return lhs->key().hash() < rhs->key().hash();
		});

	const auto* previousCommand = static_cast<const Command*>(nullptr);

	for (const auto& command : commands_) {
		command->submit(commandList, previousCommand);
		previousCommand = command.get();
	}

	commands_.clear();
	++lastFrameIdx_;
}

size_t CommandBuffer::CommandIdHash::operator()(const CommandId& commandId) const noexcept {
	auto hash = size_t();
	hash = essentials::hashCombine(hash, std::hash_value(commandId.object));
	hash = essentials::hashCombine(hash, std::hash_value(commandId.idx));
	return hash;
}

bool CommandBuffer::CommandIdEqual::operator()(const CommandId& lhs, const CommandId& rhs) const noexcept {
	return lhs.object == rhs.object && lhs.idx == rhs.idx;
}
