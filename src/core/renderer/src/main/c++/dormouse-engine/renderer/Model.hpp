#ifndef _DORMOUSEENGINE_RENDERER_MODEL_HPP_
#define _DORMOUSEENGINE_RENDERER_MODEL_HPP_

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include "dormouse-engine/milk/fs.hpp"

#include "dormouse-engine/milk/graphics/Renderer.hpp"
#include "dormouse-engine/milk/graphics/RenderState.hpp"
#include "dormouse-engine/milk/graphics/IndexBuffer.hpp"
#include "dormouse-engine/milk/graphics/VertexBuffer.hpp"
#include "dormouse-engine/milk/graphics/PrimitiveTopology.hpp"

#include "dormouse-engine/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "dormouse-engine/mesh/Mesh.hpp"
#include "dormouse-engine/mesh/MaterialConfiguration.hpp"

#include "shader/Input.hpp"
#include "shader/PassFactory.hpp"
#include "PassContext.hpp"
#include "CommandBuffer.hpp"
#include "Material.hpp"

namespace dormouse_engine::renderer {

class CommandBuffer;

class Model {
public:

	Model(
		std::string id,
		Mesh mesh,
		milk::graphics::Renderer& graphicsRenderer,
		shader::PassFactory& passFactory,
		const milk::FilesystemContext& filesystemContext
		);

	void render(CommandBuffer& commandBuffer, PassContext passContext); // TODO: make const

	const std::string& id() const {
		return id_;
	}

	// TODO: TEMP TEMP TEMP!
	Material& material() {
		return drawGroups_.front().material;
	}

private:

	struct DrawGroup {
	public:
	
		milk::graphics::VertexBuffer vertexBuffer;

		milk::graphics::IndexBuffer indexBuffer;

		milk::graphics::VertexBuffer instanceDataBuffer;

		size_t indexCount;

		milk::graphics::PrimitiveTopology primitiveTopology;

		Material material;

		DrawGroup(
			milk::graphics::Renderer& graphicsRenderer,
			shader::PassFactory& passFactory,
			const milk::fs::FilesystemContext& filesystemContext,
			mesh::Mesh::Submeshes::iterator submeshIt,
			mesh::Mesh::Submeshes::iterator submeshEnd,
			const mesh::MaterialConfiguration& materialConfiguration
			);

		void render(CommandBuffer& commandBuffer, PassContext passContext);

	};

	using DrawGroups = std::vector<DrawGroup>;

	std::string id_;

	DrawGroups drawGroups_;

};

CT_MAKE_POINTER_DEFINITIONS(Model);

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_MODEL_HPP_ */
