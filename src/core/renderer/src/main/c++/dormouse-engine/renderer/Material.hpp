#ifndef _DORMOUSEENGINE_RENDERER_MATERIAL_HPP_
#define _DORMOUSEENGINE_RENDERER_MATERIAL_HPP_

#include <string>
#include <memory>
#include <unordered_map>

#include "dormouse-engine/exceptions/RuntimeError.hpp"

#include "dormouse-engine/milk/fs/FilesystemContext.hpp"
#include "dormouse-engine/milk/graphics/Renderer.hpp"
#include "dormouse-engine/milk/graphics/RenderState.hpp"
#include "dormouse-engine/milk/graphics/Texture.hpp"
#include "dormouse-engine/milk/graphics/Sampler.hpp"
#include "dormouse-engine/mesh/MaterialConfiguration.hpp"
#include "shader/Property.hpp"
#include "shader/PassFactory.hpp"

namespace dormouse_engine::renderer {

class Material {
public:

	Material(
		milk::graphics::Renderer graphicsRenderer,
		shader::PassFactory& passFactory,
		const milk::fs::FilesystemContext& filesystemContext,
		mesh::MaterialConfiguration materialConfiguration
		);

	shader::Pass& shaderPass() {
		return *pass_;
	}

	const milk::graphics::RenderState& renderState() const {
		return renderState_;
	}

	const shader::Properties& shaderProperties() const {
		return shaderProperties_;
	}

private:

	mesh::MaterialConfiguration::PassType passType_;

	shader::PassSharedPtr pass_;

	milk::graphics::RenderState renderState_;

	shader::Properties shaderProperties_;

};

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_MATERIAL_HPP_ */
