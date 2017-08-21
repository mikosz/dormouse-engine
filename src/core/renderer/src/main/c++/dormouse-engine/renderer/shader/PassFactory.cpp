#include "PassFactory.hpp"

#include "dormouse-engine/logger.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;
using namespace dormouse_engine::renderer::shader::detail;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.SHADER.PASS_FACTORY");

} // anonymous namespace

void PassCreator::scanShaderCodeDirectory(
	const milk::fs::FilesystemContext& filesystemContext,
	const milk::fs::Path& directory,
	const std::string& /*entrypointName*/
	)
{
	CT_LOG_INFO << "Scanning shader code directory: " << directory;

	const auto names = filesystemContext.list(directory);

	for (const auto& name : names) {
		const auto path = directory / name;

		if (path.extension() == ".hlsl") {
			ShaderCreator::ShaderCodeInfo shaderInfo; // TODO: must expose this type or use different param to constructor (d'uh)
			shaderInfo.shaderCodePath = filesystemContext.makeAbsolute(path);
			shaderInfo.entrypoint = "main"; // TODO

			const auto shaderName = path.stem();

			if (shaderName.extension() == ".v") {
				shaderInfo.shaderType = graphics::ShaderType::VERTEX;
				shaderFactory_.registerShaderCode(shaderName.string(), shaderInfo);
			} else if (shaderName.extension() == ".g") {
				shaderInfo.shaderType = graphics::ShaderType::GEOMETRY;
				shaderFactory_.registerShaderCode(shaderName.string(), shaderInfo);
			} else if (shaderName.extension() == ".h") {
				shaderInfo.shaderType = graphics::ShaderType::HULL;
				shaderFactory_.registerShaderCode(shaderName.string(), shaderInfo);
			} else if (shaderName.extension() == ".d") {
				shaderInfo.shaderType = graphics::ShaderType::DOMAIN;
				shaderFactory_.registerShaderCode(shaderName.string(), shaderInfo);
			} else if (shaderName.extension() == ".p") {
				shaderInfo.shaderType = graphics::ShaderType::PIXEL;
				shaderFactory_.registerShaderCode(shaderName.string(), shaderInfo);
			}
		}
	}
}

void PassCreator::scanCompiledShaderDirectory(
	const milk::fs::FilesystemContext& filesystemContext,
	const milk::fs::Path& directory
	)
{
	CT_LOG_INFO << "Scanning compiled shader directory: " << directory;

	const auto names = filesystemContext.list(directory);

	for (const auto& name : names) {
		auto path = directory / name;
		
		if (path.extension() == ".cso") {
			ShaderCreator::CompiledShaderInfo info; // TODO: must expose this type or use different param to constructor (d'uh)
			info.compiledShaderPath = filesystemContext.makeAbsolute(path);

			const auto shaderName = path.stem();

			if (shaderName.extension() == ".v") {
				info.shaderType = graphics::ShaderType::VERTEX;
				shaderFactory_.registerCompiledShader(shaderName.string(), info);
			} else if (shaderName.extension() == ".g") {
				info.shaderType = graphics::ShaderType::GEOMETRY;
				shaderFactory_.registerCompiledShader(shaderName.string(), info);
			} else if (shaderName.extension() == ".h") {
				info.shaderType = graphics::ShaderType::HULL;
				shaderFactory_.registerCompiledShader(shaderName.string(), info);
			} else if (shaderName.extension() == ".d") {
				info.shaderType = graphics::ShaderType::DOMAIN;
				shaderFactory_.registerCompiledShader(shaderName.string(), info);
			} else if (shaderName.extension() == ".p") {
				info.shaderType = graphics::ShaderType::PIXEL;
				shaderFactory_.registerCompiledShader(shaderName.string(), info);
			}
		}
	}
}

auto PassCreator::doCreate(
	const std::string& id,
	graphics::Device& graphicsDevice,
	const milk::fs::FilesystemContext& filesystemContext
	) -> Instance
{
	CT_LOG_INFO << "Creating shader pass: \"" << id << "\"";

	auto geometryShader = GeometryShaderSharedPtr();
	auto hullShader = HullShaderSharedPtr();
	auto domainShader = DomainShaderSharedPtr();

	if (shaderFactory_.hasShader(id + ".g")) {
		geometryShader = std::dynamic_pointer_cast<GeometryShader>(
			shaderFactory_.create(id + ".g", graphicsDevice, filesystemContext));
	}
	if (shaderFactory_.hasShader(id + ".h")) {
		hullShader = std::dynamic_pointer_cast<HullShader>(
			shaderFactory_.create(id + ".h", graphicsDevice, filesystemContext));
	}
	if (shaderFactory_.hasShader(id + ".d")) {
		domainShader = std::dynamic_pointer_cast<DomainShader>(
			shaderFactory_.create(id + ".d", graphicsDevice, filesystemContext));
	}

	return std::make_unique<Pass>(
		false, // TODO: temp - need to discern between instanced and non instanced shaders
		shaderFactory_.createInput(id + ".v", graphicsDevice, filesystemContext),
		std::dynamic_pointer_cast<VertexShader>(
			shaderFactory_.create(id + ".v", graphicsDevice, filesystemContext)),
		std::move(geometryShader),
		std::move(hullShader),
		std::move(domainShader),
		std::dynamic_pointer_cast<PixelShader>(
			shaderFactory_.create(id + ".p", graphicsDevice, filesystemContext))
		);
}
