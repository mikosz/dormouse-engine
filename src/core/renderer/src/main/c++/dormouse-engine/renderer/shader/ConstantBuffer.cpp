#include "ConstantBuffer.hpp"

#include "../command/DrawCommand.hpp"

using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;

// TODO: bind, render, these names are misleading in renderer, figure out something better
void ConstantBuffer::bind(command::DrawCommand& drawCommand, const Property& root) const {
	auto bufferData = drawCommand.constantBufferData(stage_, slot_);

	for (const auto& parameter : parameters_) {
		parameter.write(bufferData, root);
	}

	drawCommand.setConstantBuffer(buffer_, stage_, slot_);
}
