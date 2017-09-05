#include "Shader.hpp"

#include "../command/DrawCommand.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;

void detail::ShaderBase::doRender(
	command::DrawCommand& cmd,
	const Properties& properties,
	graphics::ShaderType shaderType
	) const
{
	for (const auto& resource : resources_) {
		bindResource_(cmd, properties, shaderType, resource);
	}
}

void detail::ShaderBase::bindResource_(
	command::DrawCommand& cmd,
	const Properties& properties,
	graphics::ShaderType stage,
	const Resource& resource
	) const
{
	auto id = PropertyId(resource.descriptor);

	assert(!id.empty());

	const auto* currentProps = &properties;
	while (!id.tail().empty()) {
		const auto& property = currentProps->get(id.head().name);
		currentProps = &property.properties();
	}

	const auto& property = currentProps->get(id.head().name);

	const auto resourceView = currentProps->resource(id.head().name);
	cmd.setResource(resourceView, stage, resource.slot);
}
