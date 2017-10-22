#ifndef _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDBUFFER_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDBUFFER_HPP_

#include <vector>
#include <deque>
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

	// TODO: separate create and add?
	DrawCommand& create(const CommandId& commandId);

	void submit(dormouse_engine::graphics::CommandList& commandList);

private:

	// TODO: does this pooling strategy make any sense? Might as well just create
	// the commands every frame. There's probably no use in re-using them and it complicates
	// the logic.
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

	using Commands = std::vector<size_t>;

	using DrawCommandPool = std::deque<DrawCommand>;

	using DrawCommandPoolIndex =
		std::unordered_map<CommandId, DrawCommandPoolIndexEntry, CommandIdHash, CommandIdEqual>;

	Commands commands_;

	DrawCommandPool drawCommandPool_;

	DrawCommandPoolIndex drawCommandPoolIndex_;

	size_t lastFrameIdx_ = 0;

};

} // namespace dormouse_engine::renderer::command

#endif /* _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDBUFFER_HPP_ */
