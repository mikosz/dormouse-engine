#ifndef _DORMOUSEENGINE_RENDERER_COMMANDBUFFER_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMANDBUFFER_HPP_

#include <vector>
#include <cstdint>

#include "coconut/milk/graphics/CommandList.hpp"

#include "DrawCommand.hpp"

namespace coconut::pulp::renderer {

class CommandBuffer {
public:

	void add(DrawCommandUniquePtr drawCommand);

	void submit(milk::graphics::CommandList& commandList);

private:

	using Commands = std::vector<DrawCommandSharedPtr>;

	Commands commands_;

};

} // namespace coconut::pulp::renderer

#endif /* _DORMOUSEENGINE_RENDERER_COMMANDBUFFER_HPP_ */
