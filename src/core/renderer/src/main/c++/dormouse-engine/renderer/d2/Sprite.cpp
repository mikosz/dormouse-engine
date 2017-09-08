#include "Sprite.hpp"

#include <vector>

#include "dormouse-engine/graphics/Buffer.hpp"
#include "dormouse-engine/essentials/policy/creation/None.hpp"
#include "dormouse-engine/essentials/Singleton.hpp"
#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/math/Vector.hpp"
#include "../command/DrawCommand.hpp"
#include "../shader/Technique.hpp"
#include "../shader/CompoundProperty.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::d2;

namespace /* anonymous */ {

class SpriteCommon final :
	public essentials::Singleton<SpriteCommon, essentials::policy::creation::None<SpriteCommon>>
{
public:

	SpriteCommon(graphics::Device& device) :
		vertexBuffer_(createVertexBuffer(device))
	{
	}

	void render(command::DrawCommand& cmd, const Sprite& sprite) const {
		auto properties = shader::Properties();

		properties.set("sprite", essentials::make_observer(&sprite));

		technique_.render(cmd, properties);

		cmd.setVertexBuffer(vertexBuffer_, 4u);
		cmd.setPrimitiveTopology(graphics::PrimitiveTopology::TRIANGLE_STRIP);
		cmd.setTechnique(essentials::make_observer(&technique_));
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

		auto initialData = std::vector<math::Vec2> {
			{ -1.0f, -1.0f },
			{ +1.0f, +1.0f },
			{ -1.0f, +1.0f },
			{ +1.0f, +1.0f }
			};

		return graphics::Buffer(device, std::move(configuration), essentials::viewBuffer(initialData));
	}

};

} // anonymous namespace

void Sprite::initialiseSystem(graphics::Device& device) {
	SpriteCommon::setInstance(std::make_unique<SpriteCommon>(device));
}

void Sprite::render(command::CommandBuffer& commandBuffer) const {
	SpriteCommon::instance()->render(cmd_, *this);
	commandBuffer.add(essentials::make_observer(&cmd_));
}
