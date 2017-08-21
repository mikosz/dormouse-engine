#ifndef _DORMOUSEENGINE_RENDERER_COMMAND_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMAND_HPP_

#include "dormouse-engine/graphics/CommandList.hpp"
#include "CommandKey.hpp"

namespace dormouse_engine::renderer {

class Command {
public:

	virtual ~Command() = default;

	virtual CommandKey key() const = 0;

	virtual void submit(graphics::CommandList& commandList, const Command* previous) = 0;

private:

	CommandKey key_;

};

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_COMMAND_HPP_ */
