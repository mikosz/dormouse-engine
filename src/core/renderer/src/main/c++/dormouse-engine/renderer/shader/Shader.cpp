#include "Shader.hpp"

#include <algorithm>
#include <iterator>

#include <boost/tokenizer.hpp>

#include "dormouse-engine/logger.hpp"
#include "../command/DrawCommand.hpp"
#include "Property.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;

DE_LOGGER_CATEGORY("DORMOUSE_ENGINE.RENDERER.SHADER");

namespace /* anonymous */ {

PropertyDescriptor::Objects createPropertyDescriptor(const std::string& name) {
	auto result = PropertyDescriptor::Objects();

	auto separator = boost::char_separator<char>("_");
	auto tokenizer = boost::tokenizer<decltype(separator)>(name, separator);

	std::transform(
		std::make_move_iterator(tokenizer.begin()),
		std::make_move_iterator(tokenizer.end()),
		std::back_inserter(result),
		[](auto slice) {
			return essentials::StringId(std::move(slice));
		}
		);

	return result;
}

} // anonymous namespace

detail::ShaderBase::ShaderBase(essentials::ConstBufferView compiledShaderObjectData) {
	const auto reflectionData =
		graphics::ShaderReflection(compiledShaderObjectData.data(), compiledShaderObjectData.size());
	
	resources_ = createResources_(reflectionData);
}

void detail::ShaderBase::doRender(
	command::DrawCommand& cmd,
	const Property& root,
	graphics::ShaderType shaderType
	) const
{
	for (const auto& resource : resources_) {
		DE_LOG_DEBUG << "Binding resource " << resource.descriptor;
		bindResource_(cmd, root, shaderType, resource);
	}
}

auto detail::ShaderBase::createResources_(const graphics::ShaderReflection& reflectionData) -> Resources
{
	auto resources = Resources();
	resources.reserve(reflectionData.resources().size());

	for (const auto& resourceReflection : reflectionData.resources()) {
		auto resource = Resource();
		resource.descriptor = createPropertyDescriptor(resourceReflection.name);
		resource.slot = resourceReflection.slot;

		resources.emplace_back(std::move(resource));
	}

	return resources;
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
		id = id.tail();
	}

	property.bindResource(cmd, stage, resource.slot);
}
