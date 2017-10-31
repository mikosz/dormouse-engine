#include "TextureComparator.hpp"

#include <string>

#include "dormouse-engine/essentials/debug.hpp"
#include "dormouse-engine/essentials/test-utils/test-utils.hpp"
#include "dormouse-engine/graphics/ShaderCompiler.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::tester;

using namespace std::string_literals;

namespace /* anonymous */ {

graphics::ComputeShader createComparatorShader(graphics::Device& graphicsDevice) {
	const auto shaderCode = essentials::test_utils::readBinaryFile("tester/texture-comparator.hlsl");

	auto flags = graphics::ShaderCompiler::CompilerFlags();

	if (essentials::IS_DEBUG) {
		flags = graphics::ShaderCompiler::FULL_DEBUG_MASK;
	}

	auto shaderCompiler = graphics::ShaderCompiler(flags);
	const auto shaderData = shaderCompiler.compile(
		essentials::viewBuffer(shaderCode),
		"texture-comparator",
		"cs",
		graphics::ShaderType::COMPUTE
		);

	return graphics::ComputeShader(graphicsDevice, essentials::viewBuffer(shaderData));
}

} // anonymous namespace

TextureComparator::TextureComparator(graphics::Device& graphicsDevice) :
	shader_(createComparatorShader(graphicsDevice))
{
}

void TextureComparator::compare(
	graphics::Device& graphicsDevice,
	renderer::control::ResourceView reference,
	renderer::control::ResourceView actual
	) const
{
	auto& commandList = graphicsDevice.getImmediateCommandList();
	
}
