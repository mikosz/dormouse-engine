#ifndef _DORMOUSEENGINE_RENDERER_SHADER_SHADERFACTORY_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_SHADERFACTORY_HPP_

#include <mutex>
#include <memory>
#include <string>
#include <unordered_map>

#include "dormouse-engine/factory.hpp"

#include "dormouse-engine/milk/graphics/Renderer.hpp"
#include "dormouse-engine/milk/graphics/ShaderType.hpp"
#include "dormouse-engine/milk/graphics/ShaderCompiler.hpp"
#include "dormouse-engine/milk/fs.hpp"
#include "Input.hpp"
#include "Shader.hpp"

namespace dormouse_engine::renderer::shader {

namespace detail {

class ShaderCreator {
public:

	using Instance = std::unique_ptr<UnknownShader>;

	struct ShaderCodeInfo {
		milk::AbsolutePath shaderCodePath;
		milk::graphics::ShaderType shaderType;
		std::string entrypoint;
	};

	struct CompiledShaderInfo {
		milk::AbsolutePath compiledShaderPath;
		milk::graphics::ShaderType shaderType;
	};

    ShaderCreator();

	Input createInput(
		const std::string& id,
		milk::graphics::Renderer& graphicsRenderer,
		const milk::FilesystemContext& filesystemContext
		) const;

	bool hasShader(const std::string& id) const noexcept;

	void registerShaderCode(std::string id, const ShaderCodeInfo& shaderCodeInfo);

	void registerCompiledShader(std::string id, const CompiledShaderInfo& compiledShaderInfo);

protected:

	Instance doCreate(
		const std::string& id,
		milk::graphics::Renderer& graphicsRenderer,
		const milk::FilesystemContext& filesystemContext
		);

private:

	using ShaderCodeInfos = std::unordered_map<std::string, ShaderCodeInfo>;

	using CompiledShaderInfos = std::unordered_map<std::string, CompiledShaderInfo>;

    milk::graphics::ShaderCompiler shaderCompiler_;

	ShaderCodeInfos shaderCodeInfos_;

	CompiledShaderInfos compiledShaderInfos_;

	std::tuple<std::vector<std::uint8_t>, milk::graphics::ShaderType> shaderCode_(
		const std::string& id,
		const milk::FilesystemContext& filesystemContext
		) const;

};

} // namespace detail

using ShaderFactory = 
	coconut_tools::Factory<
		std::string,
		detail::ShaderCreator,
		coconut_tools::factory::storage::Volatile,
		std::mutex
		>;

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_SHADERFACTORY_HPP_ */
