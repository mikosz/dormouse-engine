#include "ShaderFactory.hpp"

#include <vector>
#include <algorithm>
#include <iterator>
#include <cctype>

#include <boost/tokenizer.hpp>

#include "dormouse-engine/logger.hpp"
#include "dormouse-engine/exceptions/RuntimeError.hpp"

#include "dormouse-engine/graphics/ShaderReflection.hpp"

#include "Property.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.SHADER_FACTORY");

using graphics::ShaderReflection;

PropertyDescriptor::Objects interpretIdentifier(const std::string& name) {
	auto result = PropertyDescriptor::Objects();

	auto separator = boost::char_separator<char>("_");
	auto tokenizer = boost::tokenizer<decltype(separator)>(name, separator);

	std::copy_if(
		std::make_move_iterator(tokenizer.begin()),
		std::make_move_iterator(tokenizer.end()),
		std::back_inserter(result),
		[](const auto& slice) {
			return std::islower(slice.front()); // Don't care about locale, only ASCII chars
		}
		);

	return result;
}

ConstantBuffer::Parameters createParameters(
	const std::string& name,
	const ShaderReflection::Type& type,
	size_t offset,
	PropertyDescriptor::Objects descriptor
	)
{
	auto parameters = ConstantBuffer::Parameters();

	{
		auto descriptorTail = interpretIdentifier(name);

		if (descriptorTail.empty()) {
			// TODO: exception
			throw coconut_tools::exceptions::RuntimeError("Invalid identifier " + name);
		}

		descriptor.insert(descriptor.end(), descriptorTail.begin(), descriptorTail.end());

		descriptor.back().arraySize = type.elements;
		descriptor.back().arrayElementOffset = type.elementOffset;
	}

	if (!type.members.empty()) {

		for (const auto& member : type.members) {
			auto memberName = std::string();
			auto memberType = ShaderReflection::Type();
			std::tie(memberName, memberType) = member;

			const auto memberParameters =
				createParameters(memberName, memberType, type.offset + offset, descriptor);
			parameters.reserve(parameters.size() + memberParameters.size());
			std::move(memberParameters.begin(), memberParameters.end(), std::back_inserter(parameters));
		}
	} else {
		CT_LOG_DEBUG << "Creating parameter at offset " << (type.offset + offset)
			<< " reading property " << PropertyDescriptor(descriptor);

		parameters.emplace_back(
			PropertyDescriptor(std::move(descriptor)),
			Property::DataType(type.klass, type.scalarType, type.columns, type.rows),
			type.offset + offset
			);
	}

	return parameters;
}

Resource createResource(
	const ShaderReflection::ResourceInfo& resourceInfo,
	graphics::ShaderType shaderType
	)
{
	auto descriptor = interpretIdentifier(resourceInfo.name);

	CT_LOG_DEBUG
		<< "Creating resource of type " << resourceInfo.type
		<< " for shader type " << shaderType
		<< " at slot " << resourceInfo.slot
		<< " reading property " << PropertyDescriptor(descriptor);

	return Resource(std::move(descriptor), resourceInfo.type, shaderType, resourceInfo.slot);
}

std::unique_ptr<UnknownShader> createShaderFromCompiledShader(
	graphics::Device& graphicsDevice,
	std::vector<std::uint8_t> shaderData,
	graphics::ShaderType shaderType
	)
{
	using graphics::ShaderReflection;

	const auto reflection = ShaderReflection(shaderData.data(), shaderData.size());

	auto constantBuffers = UnknownShader::ConstantBuffers();
	for (const auto& constantBuffer : reflection.constantBuffers()) {
		CT_LOG_DEBUG << "Building parameters for cbuffer " << constantBuffer.name;

		auto bufferParameters = ConstantBuffer::Parameters();

		for (const auto& variable : constantBuffer.variables) {
			auto variableParameters = createParameters(
				variable.name,
				variable.type,
				variable.offset,
				interpretIdentifier(constantBuffer.name)
				);
			std::move(
				variableParameters.begin(),
				variableParameters.end(),
				std::back_inserter(bufferParameters)
				);
		}

		constantBuffers.emplace_back(
			graphicsDevice,
			shaderType,
			constantBuffer.size,
			constantBuffer.slot,
			std::move(bufferParameters)
			);
	}

	auto resources = UnknownShader::Resources();
	for (const auto& resource : reflection.resources()) {
		resources.emplace_back(createResource(resource, shaderType));
	}

	switch (shaderType) {
	case graphics::ShaderType::VERTEX:
	{
		graphics::VertexShader vs(graphicsDevice, shaderData.data(), shaderData.size());
		return std::make_unique<VertexShader>(std::move(vs), std::move(constantBuffers), std::move(resources));
	}
	case graphics::ShaderType::GEOMETRY:
	{
		graphics::GeometryShader gs(graphicsDevice, shaderData.data(), shaderData.size());
		return std::make_unique<GeometryShader>(std::move(gs), std::move(constantBuffers), std::move(resources));
	}
	case graphics::ShaderType::HULL:
	{
		graphics::HullShader hs(graphicsDevice, shaderData.data(), shaderData.size());
		return std::make_unique<HullShader>(std::move(hs), std::move(constantBuffers), std::move(resources));
	}
	case graphics::ShaderType::DOMAIN:
	{
		graphics::DomainShader ds(graphicsDevice, shaderData.data(), shaderData.size());
		return std::make_unique<DomainShader>(std::move(ds), std::move(constantBuffers), std::move(resources));
	}
	case graphics::ShaderType::PIXEL:
	{
		graphics::PixelShader ps(graphicsDevice, shaderData.data(), shaderData.size());
		return std::make_unique<PixelShader>(std::move(ps), std::move(constantBuffers), std::move(resources));
	}
	}

	throw coconut_tools::exceptions::RuntimeError("Unexpected shader type: " + toString(shaderType));
}

Input createShaderInput(graphics::Device& graphicsDevice, std::vector<std::uint8_t> shaderData) {
	using graphics::ShaderReflection;
	const auto reflection = ShaderReflection(shaderData.data(), shaderData.size());

	auto inputLayoutElements = graphics::InputLayout::Elements();
	inputLayoutElements.reserve(reflection.inputParameters().size());

	auto perVertexParameters = Input::Parameters();
	auto perInstanceParameters = Input::Parameters();

	auto offset = size_t(0);

	for (const auto& inputParameter : reflection.inputParameters()) {
		auto dataType = Property::DataType();
		auto pixelFormat = graphics::PixelFormat();

		// TODO: TEMP!!! ++
		switch (inputParameter.dataType) {
		case ShaderReflection::InputParameterInfo::DataType::FLOAT:
			dataType.scalarType = Property::DataType::ScalarType::FLOAT;
			if (inputParameter.elements == 2) {
				pixelFormat = graphics::PixelFormat::R32G32_FLOAT;
			} else if (inputParameter.elements == 3) {
				pixelFormat = graphics::PixelFormat::R32G32B32_FLOAT;
			} else if (inputParameter.elements == 4) {
				pixelFormat = graphics::PixelFormat::R32G32B32A32_FLOAT;
			} else {
				assert(false);
			}
			break;
		default:
			assert(false);
		}

		dataType.columns = inputParameter.elements;
		dataType.rows = 1u;
		if (inputParameter.elements > 1) {
			dataType.klass = Property::DataType::Class::VECTOR;
		} else {
			dataType.klass = Property::DataType::Class::SCALAR;
		}
		// TODO: TEMP!!! --

		auto identifier = inputParameter.semantic + std::to_string(inputParameter.semanticIndex);
		std::transform(identifier.begin(), identifier.end(), identifier.begin(), [](char c) {
				return static_cast<char>(::tolower(c)); // Don't care about locale, only ASCII chars
			});
		auto descriptorObjects = interpretIdentifier(identifier);

		if (descriptorObjects.empty()) {
			// TODO: exception
			throw coconut_tools::exceptions::RuntimeError("Invalid semantic identifier: " + identifier);
		}

		auto inputSlot = graphics::InputLayout::SlotType();
		auto instanceDataStepRate = 0u; 

		if (descriptorObjects.front().name == "instance") {
			descriptorObjects.erase(descriptorObjects.begin());
			if (descriptorObjects.empty()) {
				// TODO: exception
				throw coconut_tools::exceptions::RuntimeError("Invalid semantic identifier: " + identifier);
			}
			perInstanceParameters.emplace_back(std::move(descriptorObjects), dataType, offset);
			inputSlot = graphics::InputLayout::SlotType::PER_INSTANCE_DATA;
			instanceDataStepRate = 1u; // TODO
		} else {
			perVertexParameters.emplace_back(std::move(descriptorObjects), dataType, offset);
			inputSlot = graphics::InputLayout::SlotType::PER_VERTEX_DATA;
		}

		inputLayoutElements.emplace_back(
			inputParameter.semantic,
			inputParameter.semanticIndex,
			pixelFormat,
			inputSlot,
			instanceDataStepRate
			);

		offset += sizeof(float) * inputParameter.elements; // TODO: temp!
	}

	perVertexParameters.shrink_to_fit();
	perInstanceParameters.shrink_to_fit();

	return Input(
		graphics::InputLayout(graphicsDevice, std::move(inputLayoutElements)),
		std::move(perVertexParameters),
		std::move(perInstanceParameters)
		);
}

} // anonymous namespace

detail::ShaderCreator::ShaderCreator() :
	// TODO: temp, take as param, need to have separate files for debug and release
	shaderCompiler_(
		// TODO: idea - could have a shorter function returning some AnyMask type which
		// has a | and & operator
		graphics::ShaderCompiler::CompilerFlags() |
		graphics::ShaderCompiler::CompilerFlag::DEBUG |
		graphics::ShaderCompiler::CompilerFlag::SKIP_OPTIMISATION
		)
{
}

Input detail::ShaderCreator::createInput(
	const std::string& id,
	graphics::Device& graphicsDevice,
	const milk::FilesystemContext& filesystemContext
	) const
{
	CT_LOG_INFO << "Creating shader input for shader: \"" << id << "\"";

	auto shaderCode = shaderCode_(id, filesystemContext);
	auto& binary = std::get<0>(shaderCode);

	assert(std::get<1>(shaderCode) == graphics::ShaderType::VERTEX);

	return createShaderInput(graphicsDevice, std::move(binary));
}

bool detail::ShaderCreator::hasShader(const std::string& id) const noexcept {
	return shaderCodeInfos_.count(id) > 0 || compiledShaderInfos_.count(id) > 0;
}

void detail::ShaderCreator::registerShaderCode(std::string id, const ShaderCodeInfo& shaderCodeInfo) {
	CT_LOG_INFO << "Registering shader code \"" << id << "\" (" << shaderCodeInfo.shaderType
		<< ") at " << shaderCodeInfo.shaderCodePath << "::" << shaderCodeInfo.entrypoint;

	if (shaderCodeInfos_.count(id) != 0 || compiledShaderInfos_.count(id) != 0) {
		throw coconut_tools::factory::error_policy::CreatorAlreadyRegistered<std::string>(id);
	}

	shaderCodeInfos_.emplace(std::move(id), std::move(shaderCodeInfo));
}

void detail::ShaderCreator::registerCompiledShader(std::string id, const CompiledShaderInfo& compiledShaderInfo) {
	CT_LOG_INFO << "Registering compiled shader \"" << id << "\" (" << compiledShaderInfo.shaderType
		<< ") at " << compiledShaderInfo.compiledShaderPath.string();

	if (shaderCodeInfos_.count(id) != 0 || compiledShaderInfos_.count(id) != 0) {
		throw coconut_tools::factory::error_policy::CreatorAlreadyRegistered<std::string>(id);
	}

	compiledShaderInfos_.emplace(std::move(id), std::move(compiledShaderInfo));
}

auto detail::ShaderCreator::doCreate(
	const std::string& id,
	graphics::Device& graphicsDevice,
	const milk::FilesystemContext& filesystemContext
	) -> Instance
{
	CT_LOG_INFO << "Creating shader: \"" << id << "\"";

	auto shaderCode = shaderCode_(id, filesystemContext);
	auto& binary = std::get<0>(shaderCode);
	auto& type = std::get<1>(shaderCode);

	return createShaderFromCompiledShader(graphicsDevice, std::move(binary), std::move(type));
}

std::tuple<std::vector<std::uint8_t>, graphics::ShaderType> detail::ShaderCreator::shaderCode_(
	const std::string& id,
	const milk::FilesystemContext& filesystemContext
	) const
{
	if (shaderCodeInfos_.count(id) != 0) {
		const auto& shaderInfo = shaderCodeInfos_.at(id);

		CT_LOG_DEBUG << "Found \"" << id << "\" registered as shader code";
		CT_LOG_INFO << "Compiling shader at " << shaderInfo.shaderCodePath << "::" << shaderInfo.entrypoint;

		const auto shaderCode = filesystemContext.load(shaderInfo.shaderCodePath);

		const auto shaderDir = shaderInfo.shaderCodePath.parent();
		const auto includeFSContext = filesystemContext.createContext(shaderDir);
		auto shaderIncludeHandler = [&includeFSContext](const auto& path) {
				return includeFSContext.load(path);
			};
		auto shaderBytecode = shaderCompiler_.compile(
			*shaderCode,
			shaderInfo.shaderCodePath.string(),
			shaderInfo.entrypoint,
			shaderInfo.shaderType,
			std::move(shaderIncludeHandler)
			);

		return std::make_tuple(
			std::move(shaderBytecode),
			shaderInfo.shaderType
			);
	} else if (compiledShaderInfos_.count(id) != 0) {
		const auto& shaderInfo = compiledShaderInfos_.at(id);

		CT_LOG_DEBUG << "Found \"" << id << "\" registered as a compiled shader";

		return std::make_tuple(*filesystemContext.load(shaderInfo.compiledShaderPath), shaderInfo.shaderType);
	} else {
		throw coconut_tools::factory::error_policy::NoSuchType<std::string>(id);
	}
}
