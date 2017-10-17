#include "CommandBuffer.hpp"

#include <algorithm>

using namespace dormouse_engine;
using namespace dormouse_engine::renderer::command;

DrawCommand& CommandBuffer::create(const CommandId& commandId) {
	auto it = drawCommandPoolIndex_.find(commandId);

	if (it != drawCommandPoolIndex_.end()) {
		it->second.lastFrameUsed = lastFrameIdx_ + 1;
		commands_.emplace_back(it->second.poolIndex);
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
				commands_.emplace_back(poolIndex);
				return drawCommandPool_[poolIndex];
			}
		}

		drawCommandPool_.emplace_back();
		const auto poolIndex = drawCommandPool_.size() - 1;
		drawCommandPoolIndex_.emplace(
			commandId, DrawCommandPoolIndexEntry{ poolIndex, lastFrameIdx_ + 1 });
		commands_.emplace_back(poolIndex);
		return drawCommandPool_.back();
	}
}

void CommandBuffer::submit(dormouse_engine::graphics::CommandList& commandList) {
	std::sort(commands_.begin(), commands_.end(), [this](const auto lhs, const auto rhs) {
			return drawCommandPool_[lhs].key().hash() < drawCommandPool_[rhs].key().hash();
		});

	const auto* previousCommand = static_cast<const Command*>(nullptr);

	for (const auto poolIdx : commands_) {
		const auto command = &drawCommandPool_[poolIdx];
		command->submit(commandList, previousCommand);
		previousCommand = command;
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
