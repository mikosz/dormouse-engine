#ifndef _DORMOUSEENGINE_RENDERER_DRAWCOMMAND_HPP_
#define _DORMOUSEENGINE_RENDERER_DRAWCOMMAND_HPP_

#include "dormouse-engine/graphics/CommandList.hpp"
#include "CommandKey.hpp"

namespace dormouse_engine::renderer {

class DrawCommand {
public:

	CommandKey key() const {
		return key_;
	}

	void submit(graphics::CommandList& commandList, const DrawCommand* previous);

private:

	CommandKey key_;

};

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_DRAWCOMMAND_HPP_ */
