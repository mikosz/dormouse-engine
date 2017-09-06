#include "Shader.hpp"

#include "../command/DrawCommand.hpp"
#include "PropertyObject.hpp"
#include "Properties.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;

void detail::ShaderBase::doRender(
	command::DrawCommand& cmd,
	const PropertyObject& root,
	graphics::ShaderType shaderType
	) const
{
	for (const auto& resource : resources_) {
		bindResource_(cmd, root, shaderType, resource);
	}
}

void detail::ShaderBase::bindResource_(
	command::DrawCommand& cmd,
	const PropertyObject& root,
	graphics::ShaderType stage,
	const Resource& resource
	) const
{
	auto id = PropertyId(resource.descriptor);

	assert(!id.empty());

	const auto* object = &root;

	while (!id.tail().empty()) {
		object = &object->properties().get(id.head().name);
	}

	const auto resourceView = object->resource(id.head().name);
	cmd.setResource(resourceView, stage, resource.slot);
}
