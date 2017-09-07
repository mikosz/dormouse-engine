#ifndef _DORMOUSEENGINE_RENDERER_COMMAND_COMMAND_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMAND_COMMAND_HPP_

#include "dormouse-engine/graphics/CommandList.hpp"
#include "CommandKey.hpp"

namespace dormouse_engine::renderer::command {

class Command {
public:

	virtual ~Command() = default;

	virtual CommandKey key() const = 0;

	virtual void submit(graphics::CommandList& commandList, const Command* previous) const = 0;

};

} // namespace dormouse_engine::renderer::command

#endif /* _DORMOUSEENGINE_RENDERER_COMMAND_COMMAND_HPP_ */
