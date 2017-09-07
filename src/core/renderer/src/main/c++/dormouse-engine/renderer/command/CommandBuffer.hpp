#ifndef _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDBUFFER_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDBUFFER_HPP_

#include <vector>

#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/graphics/CommandList.hpp"
#include "Command.hpp"

namespace dormouse_engine::renderer::command {

class CommandBuffer final {
public:

	void add(Command command) {
		commands_.emplace_back(std::move(command));
	}

	void submit(dormouse_engine::graphics::CommandList& commandList);

private:

	using Commands = std::vector<Command>;

	Commands commands_;

};

} // namespace dormouse_engine::renderer::command

#endif /* _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDBUFFER_HPP_ */
