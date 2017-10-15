#ifndef _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDBUFFER_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDBUFFER_HPP_

#include <vector>
#include <unordered_map>

#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/essentials/hash-combine.hpp"
#include "dormouse-engine/graphics/CommandList.hpp"
#include "DrawCommand.hpp"

namespace dormouse_engine::renderer::command {

class CommandBuffer final {
public:

	struct CommandId final {
		const void* object;
		size_t idx;
	};

	essentials::observer_ptr<DrawCommand> get(const CommandId& commandId);

	void add(essentials::observer_ptr<const Command> command) {
		// TODO: can't take observer_ptr here, must be size_t, index to pool, otherwise a get that
		// grows the vector will invalidate the pointer!
		commands_.emplace_back(std::move(command));
	}

	void submit(dormouse_engine::graphics::CommandList& commandList);

private:

	struct DrawCommandPoolIndexEntry {
		size_t poolIndex;
		size_t lastFrameUsed;
	};

	struct CommandIdHash {
		size_t operator()(const CommandId& commandId) const noexcept;
	};

	struct CommandIdEqual {
		bool operator()(const CommandId& lhs, const CommandId& rhs) const noexcept;
	};

	using Commands = std::vector<essentials::observer_ptr<const Command>>;

	using DrawCommandPool = std::vector<DrawCommand>;

	using DrawCommandPoolIndex =
		std::unordered_map<CommandId, DrawCommandPoolIndexEntry, CommandIdHash, CommandIdEqual>;

	Commands commands_;

	DrawCommandPool drawCommandPool_;

	DrawCommandPoolIndex drawCommandPoolIndex_;

	size_t lastFrameIdx_ = 0;

};

} // namespace dormouse_engine::renderer::command

#endif /* _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDBUFFER_HPP_ */
