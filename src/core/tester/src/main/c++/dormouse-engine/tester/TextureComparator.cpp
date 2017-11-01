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

graphics::Buffer createGPUResultBuffer(graphics::Device& graphicsDevice) {
	auto configuration = graphics::Buffer::Configuration();
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = true;
	configuration.allowModifications = false;
	configuration.purpose = graphics::Buffer::CreationPurpose::UNORDERED_ACCESS;
	configuration.size = sizeof(bool);

	return graphics::Buffer(graphicsDevice, configuration);
}

graphics::Buffer createCPUResultBuffer(graphics::Device& graphicsDevice) {
	auto configuration = graphics::Buffer::Configuration();
	configuration.allowCPURead = true;
	configuration.allowGPUWrite = true;
	configuration.allowModifications = true;
	configuration.size = sizeof(bool);

	return graphics::Buffer(graphicsDevice, configuration);
}

} // anonymous namespace

TextureComparator::TextureComparator(graphics::Device& graphicsDevice) :
	shader_(createComparatorShader(graphicsDevice)),
	resultGPU_(createGPUResultBuffer(graphicsDevice)),
	resultCPU_(createCPUResultBuffer(graphicsDevice)),
	resultUAV_(resultGPU_, graphics::FORMAT_R8_UINT, 0, 1)
{
}

bool TextureComparator::compare(
	graphics::Device& graphicsDevice,
	const graphics::ResourceView& reference,
	const graphics::ResourceView& actual,
	size_t width,
	size_t height
	) const
{
	auto& commandList = graphicsDevice.getImmediateCommandList();
	
	{
		const auto lockedResult = commandList.lock(resultGPU_, graphics::CommandList::LockPurpose::WRITE_DISCARD);
		*lockedResult.pixels = true;
	}

	commandList.setResource(reference, graphics::ShaderType::COMPUTE, 0);
	commandList.setResource(actual, graphics::ShaderType::COMPUTE, 1);
	commandList.setUnorderedAccessView(resultUAV_, 0);

	commandList.dispatch((width + 15) / 16, (height + 15) / 16, 1);

	commandList.copy(resultGPU_, resultCPU_);

	{
		const auto lockedResult = commandList.lock(resultCPU_, graphics::CommandList::LockPurpose::READ);
		return *lockedResult.pixels;
	}
}
