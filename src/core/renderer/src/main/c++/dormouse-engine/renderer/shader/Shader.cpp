#include "Shader.hpp"

#include "../command/DrawCommand.hpp"
#include "Property.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;

void detail::ShaderBase::doRender(
	command::DrawCommand& cmd,
	const Property& root,
	graphics::ShaderType shaderType
	) const
{
	for (const auto& resource : resources_) {
		bindResource_(cmd, root, shaderType, resource);
	}
}

void detail::ShaderBase::bindResource_(
	command::DrawCommand& cmd,
	const Property& root,
	graphics::ShaderType stage,
	const Resource& resource
	) const
{
	auto id = PropertyId(resource.descriptor);

	assert(!id.empty());

	auto property = root;

	while (!id.tail().empty()) {
		property = property.get(id.head().name);
	}

	const auto resourceView = property.as<control::ResourceView>();
	cmd.setResource(resourceView, stage, resource.slot);
}
