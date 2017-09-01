#include "Technique.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer::shader;

void Technique::bind(graphics::CommandList& commandList) const
{
	vertexShader_.bind(commandList);
	geometryShader_.bind(commandList);
	domainShader_.bind(commandList);
	hullShader_.bind(commandList);
	pixelShader_.bind(commandList);
}
