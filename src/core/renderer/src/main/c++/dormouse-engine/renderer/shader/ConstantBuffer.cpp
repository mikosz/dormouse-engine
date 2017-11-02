#include "ConstantBuffer.hpp"

#include "../command/DrawCommand.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;

namespace /* anonymous */ {

graphics::Buffer createConstantBuffer(graphics::Device& graphicsDevice, size_t size) {
	auto configuration = graphics::Buffer::Configuration();
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowCPUWrite = true;
	configuration.purpose = graphics::Buffer::Purpose::CONSTANT_BUFFER;
	configuration.size = size;
	return graphics::Buffer(graphicsDevice, configuration);
}

} // anonymous namespace

ConstantBuffer::ConstantBuffer(
	graphics::Device& graphicsDevice,
	graphics::ShaderType stage,
	size_t slot,
	size_t size,
	Parameters parameters
	) :
	buffer_(createConstantBuffer(graphicsDevice, size)),
	stage_(stage),
	slot_(slot),
	size_(size),
	parameters_(std::move(parameters))
{
}

// TODO: bind, render, these names are misleading in renderer, figure out something better
void ConstantBuffer::bind(command::DrawCommand& drawCommand, const Property& root) const {
	drawCommand.setConstantBuffer(buffer_, stage_, slot_);

	auto& buffer = drawCommand.constantBufferData(stage_, slot_);
	buffer.resize(size_);

	for (const auto& parameter : parameters_) {
		parameter.write(essentials::viewBuffer(buffer), root);
	}
}
