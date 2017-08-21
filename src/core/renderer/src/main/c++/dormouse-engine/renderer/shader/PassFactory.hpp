#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PASSFACTORY_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PASSFACTORY_HPP_

#include <mutex>
#include <memory>

#include "dormouse-engine/factory.hpp"
#include "dormouse-engine/graphics/Device.hpp"
#include "ShaderFactory.hpp"
#include "Pass.hpp"

namespace dormouse_engine::renderer::shader {

namespace detail {

class PassCreator {
public:

	using Instance = std::unique_ptr<Pass>;

	ShaderFactory& shaderFactory() noexcept {
		return shaderFactory_;
	}

protected:

	Instance doCreate(
		const std::string& id,
		graphics::Device& graphicsDevice
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
