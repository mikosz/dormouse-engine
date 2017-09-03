#include "Shader.hpp"

#include "../command/DrawCommand.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;

void detail::ShaderBase::doRender(
	command::DrawCommand& cmd, const Properties& properties, graphics::ShaderType shaderType) const
{
	for (const auto& resource : resources_) {
		properties.bindResource(cmd, resource.descriptor, shaderType, resource.slot);
	}
}
