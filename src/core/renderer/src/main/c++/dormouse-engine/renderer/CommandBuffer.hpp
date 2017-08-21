#ifndef _DORMOUSEENGINE_RENDERER_COMMANDBUFFER_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMANDBUFFER_HPP_

#include <vector>
#include <cstdint>

#include "dormouse-engine/graphics/CommandList.hpp"

#include "DrawCommand.hpp"

namespace dormouse_engine::renderer {

class CommandBuffer {
public:

	void add(DrawCommandUniquePtr drawCommand);

	void submit(graphics::CommandList& commandList);

private:

	using Commands = std::vector<DrawCommandSharedPtr>;

	Commands commands_;

};

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_COMMANDBUFFER_HPP_ */
