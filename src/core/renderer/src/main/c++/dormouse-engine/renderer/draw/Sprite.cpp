#include "Sprite.hpp"

#include "../command/DrawCommand.hpp"

using namespace dormouse_engine::renderer::draw;

Sprite::Sprite(const graphics::Texture& texture) :
	textureView_(texture)
{
}

void Sprite::render(command::CommandBuffer& commandBuffer) const {
	auto cmd = command::DrawCommand();

	commandBuffer.add(std::move(cmd));
}
