#include "Shader.hpp"

#include <algorithm>
#include <iterator>

#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/move/iterator.hpp>
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

	boost::copy(
		tokenizer |
		boost::adaptors::filtered([](const auto& slice) { return std::islower(slice.front()); } ) |
		boost::adaptors::transformed([](const auto& slice) { return essentials::StringId(std::move(slice)); } ),
		boost::back_move_inserter(result)
		);

	return result;
}

ConstantBuffer::Parameters createParameters(
	const std::string& name,
	const graphics::ShaderReflection::Type& type,
	size_t offset,
	PropertyDescriptor::Objects descriptor
	)
{
	auto parameters = ConstantBuffer::Parameters();

	{
		auto descriptorTail = createPropertyDescriptor(name);

		if (descriptorTail.empty()) {
			// TODO: exception
			throw exceptions::RuntimeError("Invalid identifier " + name);
		}

		descriptor.insert(
			descriptor.end(),
			std::make_move_iterator(descriptorTail.begin()),
			std::make_move_iterator(descriptorTail.end())
			);

		descriptor.back().arraySize = type.elements;
		descriptor.back().arrayElementOffset = type.elementOffset;
	}

	if (!type.members.empty()) {
		for (const auto& member : type.members) {
			auto memberName = std::string();
			auto memberType = graphics::ShaderReflection::Type();
			std::tie(memberName, memberType) = member;

			const auto memberParameters =
				createParameters(memberName, memberType, type.offset + offset, descriptor);
			parameters.reserve(parameters.size() + memberParameters.size());
			std::move(memberParameters.begin(), memberParameters.end(), std::back_inserter(parameters));
		}
	} else {
		DE_LOG_DEBUG << "Creating parameter at offset " << (type.offset + offset)
			<< " reading property " << PropertyDescriptor(descriptor);

		parameters.emplace_back(
			PropertyDescriptor(std::move(descriptor)),
			Parameter::DataType(type.klass, type.scalarType, type.columns, type.rows),
			type.offset + offset
			);
	}

	return parameters;
}

} // anonymous namespace

detail::ShaderBase::ShaderBase(
	graphics::Device& graphicsDevice,
	graphics::ShaderType shaderType,
	essentials::ConstBufferView compiledShaderObjectData
	)
{
	const auto reflectionData =
		graphics::ShaderReflection(compiledShaderObjectData.data(), compiledShaderObjectData.size());
	
	resources_ = createResources_(reflectionData);
	constantBuffers_ = createConstantBuffers_(graphicsDevice, shaderType, reflectionData);
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
		cb.bind(cmd, root);
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

auto detail::ShaderBase::createConstantBuffers_(
	graphics::Device& graphicsDevice,
	graphics::ShaderType shaderType,
	const graphics::ShaderReflection& reflectionData
	) -> ConstantBuffers
{
	auto buffers = ConstantBuffers();
	buffers.reserve(reflectionData.constantBuffers().size());

	for (const auto& cbReflection : reflectionData.constantBuffers()) {
		auto parameters = ConstantBuffer::Parameters();

		for (const auto& variable : cbReflection.variables) {
			auto variableParameters = createParameters(
				variable.name,
				variable.type,
				variable.offset,
				createPropertyDescriptor(cbReflection.name)
				);
			std::move(
				variableParameters.begin(),
				variableParameters.end(),
				std::back_inserter(parameters)
				);
		}

		buffers.emplace_back(
			graphicsDevice, shaderType, cbReflection.slot, cbReflection.size, std::move(parameters));
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
