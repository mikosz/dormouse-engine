#include "InputLayout.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer::shader;

namespace /*anonymous */ {

graphics::InputLayout createInputLayout(
	graphics::Device& graphicsDevice,
	essentials::ConstBufferView compiledVertexShaderObjectData
	)
{
	const auto reflectionData = graphics::ShaderReflection( // TODO: ShaderReflection constructor should accept ConstBufferView
		compiledVertexShaderObjectData.data(), compiledVertexShaderObjectData.size());

	for (const auto& inputParameter : reflectionData.inputParameters()) {

	}
}

} // anonymous namespace

InputLayout::InputLayout(
	graphics::Device& graphicsDevice,
	essentials::ConstBufferView compiledVertexShaderObjectData
	) :
	inputLayout_(createInputLayout(graphicsDevice, compiledVertexShaderObjectData))
{
}
