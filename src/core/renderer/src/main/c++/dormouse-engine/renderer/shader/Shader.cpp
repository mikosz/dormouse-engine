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
		DE_LOG_TRACE << "Binding resource " << resource.descriptor;
		bindResource_(cmd, root, shaderType, resource);
	}

	for (const auto& cb : constantBuffers_) {
		DE_LOG_TRACE << "Binding constant buffer " << cb.descriptor;
		bindConstantBuffer_(cmd, root, shaderType, cb);
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

auto detail::ShaderBase::createConstantBuffers_(const graphics::ShaderReflection& reflectionData) -> ConstantBuffers
{
	auto buffers = ConstantBuffers();
	buffers.reserve(reflectionData.constantBuffers().size());

	for (const auto& cbReflection : reflectionData.constantBuffers()) {
		auto cb = ConstantBuffer();
		cb.descriptor = createPropertyDescriptor(cbReflection.name);
		cb.slot = cbReflection.slot;

		buffers.emplace_back(std::move(cb));
	}

	return buffers;
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

	while (!id.empty()) {
		if (!property.has(id.head().name)) {
			throw PropertyNotBound(resource.descriptor);
		}
		property = property.get(id.head().name);
		id = id.tail();
	}

	property.bindResource(cmd, stage, resource.slot);
}
