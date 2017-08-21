#ifndef _DORMOUSEENGINE_RENDERER_SCENE_HPP_
#define _DORMOUSEENGINE_RENDERER_SCENE_HPP_

#include <vector>

#include "dormouse-engine/essentials/smart-pointer-definitions.hpp"
#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Viewport.hpp"
#include "shader/ReflectiveObject.hpp"
#include "shader/Pass.hpp"
#include "lighting/DirectionalLight.hpp"
#include "lighting/PointLight.hpp"
#include "PassContext.hpp"
#include "Actor.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "Lens.hpp"

namespace dormouse_engine::renderer {

class CommandBuffer;

class Scene {
public:

	Scene(graphics::Device& graphicsDevice);

	void add(ActorSharedPtr actor, ModelSharedPtr model);

	void add(lighting::DirectionalLight directionalLight);

	void add(lighting::PointLight pointLight);

	void setCamera(CameraSharedPtr camera);

	void setLens(LensSharedPtr lens);

	void render(PassContext passContext, CommandBuffer& commandBuffer);

	const Camera& camera() const {
		return *camera_;
	}

	const Lens& lens() const {
		return *lens_;
	}

	const std::vector<lighting::DirectionalLight>& directionalLights() const {
		return directionalLights_;
	}

	const std::vector<lighting::PointLight>& pointLights() const {
		return pointLights_;
	}

private:

	struct Instance {

		ModelSharedPtr model;

		std::vector<ActorSharedPtr> actors;

		// graphics::VertexBuffer instanceDataBuffer; // TODO: move from model?

		Instance(ModelSharedPtr model) :
			model(std::move(model))
		{
		}

	};

	std::unordered_map<std::string, Instance> instances_;

	std::vector<lighting::DirectionalLight> directionalLights_;

	std::vector<lighting::PointLight> pointLights_;

	CameraSharedPtr camera_; // TODO
	
	LensSharedPtr lens_; // TODO

	graphics::Texture2d* renderTarget_; // TODO

	graphics::Texture2d* depthStencil_; // TODO

	graphics::Viewport viewport_; // TODO

	friend class renderer::shader::ReflectiveInterface<Scene>;

};

DE_SMART_POINTER_DEFINITONS(Scene);

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_SCENE_HPP_ */
