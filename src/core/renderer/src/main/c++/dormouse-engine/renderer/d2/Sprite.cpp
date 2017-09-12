#include "Sprite.hpp"

#include <vector>

#include "dormouse-engine/graphics/Buffer.hpp"
#include "dormouse-engine/graphics/ShaderCompiler.hpp"
#include "dormouse-engine/essentials/policy/creation/None.hpp"
#include "dormouse-engine/essentials/Singleton.hpp"
#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/math/Vector.hpp"
#include "../control/Sampler.hpp"
#include "../control/RenderState.hpp"
#include "../command/DrawCommand.hpp"
#include "../shader/Technique.hpp"
#include "../shader/MergedProperty.hpp"
#include "../shader/NamedProperty.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::d2;

namespace /* anonymous */ {

class SpriteCommon final :
	public essentials::Singleton<SpriteCommon, essentials::policy::creation::None<SpriteCommon>>
{
public:

	SpriteCommon(graphics::Device& graphicsDevice, essentials::ConstBufferView shaderCode) :
		vertexBuffer_(createVertexBuffer_(graphicsDevice)),
		technique_(createTechnique_(graphicsDevice, std::move(shaderCode))),
		sampler_(graphicsDevice, control::Sampler::CLAMPED_LINEAR),
		renderState_(graphicsDevice, control::RenderState::OPAQUE)
	{
	}

	void render(command::DrawCommand& cmd, const Sprite& sprite, const shader::Property& properties) const {
		auto spriteProperty = shader::Property(essentials::make_observer(&sprite));
		auto spriteEntry = shader::Property(shader::NamedProperty("sprite", essentials::make_observer(&spriteProperty)));

		auto mergedProperty = shader::MergedProperty(
			essentials::make_observer(&spriteEntry),
			essentials::make_observer(&properties)
			);

		technique_.render(cmd, mergedProperty);

		cmd.setRenderState(renderState_);
		cmd.setVertexBuffer(vertexBuffer_, 4u);
		cmd.setPrimitiveTopology(graphics::PrimitiveTopology::TRIANGLE_STRIP);
		cmd.setTechnique(essentials::make_observer(&technique_));
	}

	const control::Sampler& sampler() const {
		return sampler_;
	}

private:

	const graphics::Buffer vertexBuffer_;

	const shader::Technique technique_;

	const control::Sampler sampler_;

	const control::RenderState renderState_;

	static graphics::Buffer createVertexBuffer_(graphics::Device& graphicsDevice) {
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

		return graphics::Buffer(graphicsDevice, std::move(configuration), essentials::viewBuffer(initialData));
	}

	static shader::Technique createTechnique_(
		graphics::Device& graphicsDevice, essentials::ConstBufferView shaderCode)
	{
		auto technique = shader::Technique();

		auto shaderCompiler = graphics::ShaderCompiler();

		{
			auto compiledVertexShader =
				shaderCompiler.compile(shaderCode, "sprite", "vs", graphics::ShaderType::VERTEX);
			technique.setShader(
				shader::VertexShader(graphicsDevice, essentials::viewBuffer(compiledVertexShader)));
		}

		{
			auto compiledPixelShader =
				shaderCompiler.compile(shaderCode, "sprite", "ps", graphics::ShaderType::PIXEL);
			technique.setShader(
				shader::PixelShader(graphicsDevice, essentials::viewBuffer(compiledPixelShader)));
		}

		return technique;
	}

};

} // anonymous namespace

void Sprite::initialiseSystem(graphics::Device& device, essentials::ConstBufferView shaderCode) {
	SpriteCommon::setInstance(std::make_unique<SpriteCommon>(device, std::move(shaderCode)));
}

void Sprite::render(command::CommandBuffer& commandBuffer, const shader::Property& properties) const {
	SpriteCommon::instance()->render(cmd_, *this, properties);
	commandBuffer.add(essentials::make_observer(&cmd_));
}

bool d2::hasShaderProperty(const Sprite& /*model*/, essentials::StringId id)
{
	if (id == essentials::StringId("texture")) {
		return true;
	} else if (id == essentials::StringId("sampler")) {
		return true;
	}

	return false;
}

shader::Property d2::getShaderProperty(const Sprite& model, essentials::StringId id)
{
	if (id == essentials::StringId("texture")) {
		return model.texture();
	} else if (id == essentials::StringId("sampler")) {
		return SpriteCommon::instance()->sampler();
	}

	assert(!"Property not bound");
	return shader::Property();
}
