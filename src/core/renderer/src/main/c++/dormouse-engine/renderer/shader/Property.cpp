#include "Property.hpp"

#include "../command/DrawCommand.hpp"
#include "../control/ResourceView.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;

void shader::bindShaderResource(
	control::ResourceView resourceView, command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot)
{
	cmd.setResource(std::move(resourceView), stage, slot);
}
