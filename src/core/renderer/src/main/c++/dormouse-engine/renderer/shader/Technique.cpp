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

void Technique::render(command::DrawCommand& cmd, const PropertyObject& root) const
{
	vertexShader_.render(cmd, root);
	geometryShader_.render(cmd, root);
	domainShader_.render(cmd, root);
	hullShader_.render(cmd, root);
	pixelShader_.render(cmd, root);
}
