#ifndef _DORMOUSEENGINE_RENDERER_COMMANDBUFFER_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMANDBUFFER_HPP_

#include <tuple>
#include <vector>

#include "dormouse-engine/graphics/CommandList.hpp"
#include "DrawCommand.hpp"

namespace dormouse_engine::renderer {

class CommandBuffer {
public:

	void add(DrawCommand drawCommand) {
		index_.emplace_back(drawCommand.key(), commands_.size());
		commands_.emplace_back(std::move(drawCommand));
	}

	void submit(dormouse_engine::graphics::CommandList& commandList);

private:

	using Index = std::vector<std::tuple<CommandKey, size_t>>;

	using Commands = std::vector<DrawCommand>;

	Index index_;

	Commands commands_;

};

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_COMMANDBUFFER_HPP_ */
