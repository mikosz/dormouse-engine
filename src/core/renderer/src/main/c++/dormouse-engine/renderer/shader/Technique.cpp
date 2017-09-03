#include "Technique.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;

void Technique::bind(graphics::CommandList& commandList) const {
	vertexShader_.bind(commandList);
	geometryShader_.bind(commandList);
	domainShader_.bind(commandList);
	hullShader_.bind(commandList);
	pixelShader_.bind(commandList);
}

void Technique::render(command::DrawCommand& cmd, const Properties& properties) const
{
	vertexShader_.render(cmd, properties);
	geometryShader_.render(cmd, properties);
	domainShader_.render(cmd, properties);
	hullShader_.render(cmd, properties);
	pixelShader_.render(cmd, properties);
}
