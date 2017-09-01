#include "Sprite.hpp"

#include <vector>

#include "dormouse-engine/graphics/Buffer.hpp"
#include "dormouse-engine/essentials/policy/creation/None.hpp"
#include "dormouse-engine/essentials/Singleton.hpp"
#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/math/Vector.hpp"
#include "../command/DrawCommand.hpp"
#include "../shader/Technique.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::draw;

namespace /* anonymous */ {

class SpriteData final :
	public essentials::Singleton<SpriteData, essentials::policy::creation::None<SpriteData>>
{
public:

	SpriteData(graphics::Device& device) :
		vertexBuffer_(createVertexBuffer(device))
	{
	}

	void bind(command::DrawCommand& cmd) const {
		cmd.setVertexBuffer(vertexBuffer_);
		cmd.setTechnique(technique_);
	}

private:

	graphics::Buffer vertexBuffer_;

	shader::Technique technique_;

	static graphics::Buffer createVertexBuffer(graphics::Device& device) {
		auto configuration = graphics::Buffer::Configuration();

		configuration.allowCPURead = false;
		configuration.allowGPUWrite = false;
		configuration.allowModifications = false;
		configuration.purpose = graphics::Buffer::CreationPurpose::VERTEX_BUFFER;
		configuration.size = 4u;
		configuration.stride = 0u;

		auto initialData = std::vector<math::Vec3>(4u);

		return graphics::Buffer(device, configuration, initialData);
	}

};

} // anonymous namespace

void Sprite::initialiseSystem(graphics::Device& device) {
	SpriteData::setInstance(std::make_unique<SpriteData>(device));
}

Sprite::Sprite(const graphics::Texture& texture) :
	textureView_(texture)
{
}

void Sprite::render(command::CommandBuffer& commandBuffer) const {
	auto cmd = command::DrawCommand();

	auto technique = Technique();
	technique.setShader(vertexShader_);
	technique.setShader(std::move)
		
	SpriteData::instance()->bind(cmd);

	commandBuffer.add(std::move(cmd));
}
