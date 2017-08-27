#ifndef _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDBUFFER_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDBUFFER_HPP_

#include <tuple>
#include <vector>

#include "dormouse-engine/graphics/CommandList.hpp"
#include "Command.hpp"

namespace dormouse_engine::renderer::command {

class CommandBuffer final {
public:

	void add(Command command) {
		index_.emplace_back(command.key(), commands_.size());
		commands_.emplace_back(std::move(command));
	}

	void submit(dormouse_engine::graphics::CommandList& commandList);

private:

	using Index = std::vector<std::tuple<CommandKey, size_t>>;

	using Commands = std::vector<Command>;

	Index index_;

	Commands commands_;

};

} // namespace dormouse_engine::renderer::command

#endif /* _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDBUFFER_HPP_ */
