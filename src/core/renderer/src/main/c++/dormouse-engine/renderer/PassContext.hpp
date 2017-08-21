#ifndef _DORMOUSEENGINE_RENDERER_PASSCONTEXT_HPP_
#define _DORMOUSEENGINE_RENDERER_PASSCONTEXT_HPP_

#include <string>
#include <vector>

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Texture2d.hpp"
#include "dormouse-engine/graphics/Viewport.hpp"

#include "dormouse-engine/mesh/MaterialConfiguration.hpp"

#include "shader/Pass.hpp"
#include "shader/Property.hpp"

namespace dormouse_engine::renderer {

class Scene;
class Actor;
class Model;
class Material;

// TODO: I don't like these pointers, move fields to private, stop passing this as value
class PassContext {
public:

	graphics::Device* graphicsDevice = nullptr; // TODO: only needed if Model updates the instance data buffer

	graphics::Viewport* viewport = nullptr;

	graphics::Texture2d* backBuffer = nullptr;

	graphics::Texture2d* screenDepthStencil = nullptr;

	const Scene* scene = nullptr;

	const std::vector<std::shared_ptr<Actor>>* actors = nullptr;

	const Model* model = nullptr;

	const Material* material = nullptr;

	std::string shader;

	mesh::MaterialConfiguration::PassType passType; // TODO: I don't like this

	// TODO: cleanup!
	shader::Properties properties;

	void reset() {
		graphicsDevice = nullptr;
		viewport = nullptr;
		backBuffer = nullptr;
		screenDepthStencil = nullptr;
		scene = nullptr;
		actors = nullptr;
		model = nullptr;
		material = nullptr;
		shader.clear();
	}

};

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_PASSCONTEXT_HPP_ */
