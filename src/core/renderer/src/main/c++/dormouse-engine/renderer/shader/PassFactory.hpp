#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PASSFACTORY_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PASSFACTORY_HPP_

#include <mutex>
#include <memory>

#include "dormouse-engine/factory.hpp"

#include "dormouse-engine/milk/graphics/Renderer.hpp"

#include "dormouse-engine/milk/fs.hpp"

#include "ShaderFactory.hpp"
#include "Pass.hpp"

namespace dormouse_engine::renderer::shader {

namespace detail {

class PassCreator {
public:

	using Instance = std::unique_ptr<Pass>;

	void scanShaderCodeDirectory(
		const milk::fs::FilesystemContext& filesystemContext,
		const milk::fs::Path& directory,
		const std::string& entrypointName = "main");

	void scanCompiledShaderDirectory(
		const milk::fs::FilesystemContext& filesystemContext,
		const milk::fs::Path& directory
		);

	ShaderFactory& shaderFactory() noexcept {
		return shaderFactory_;
	}

protected:

	Instance doCreate(
		const std::string& id,
		milk::graphics::Renderer& graphicsRenderer,
		const milk::fs::FilesystemContext& filesystemContext
	);

private:

	ShaderFactory shaderFactory_;

};

} // namespace detail

using PassFactory = 
	coconut_tools::Factory<
		std::string,
		detail::PassCreator,
		coconut_tools::factory::storage::Volatile,
		std::mutex
		>;

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PASSFACTORY_HPP_ */
