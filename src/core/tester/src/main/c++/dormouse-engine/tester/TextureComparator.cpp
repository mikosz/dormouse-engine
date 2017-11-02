#include "TextureComparator.hpp"

#include <string>

#include "dormouse-engine/essentials/debug.hpp"
#include "dormouse-engine/essentials/test-utils/test-utils.hpp"
#include "dormouse-engine/graphics/ShaderCompiler.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::tester;

using namespace std::string_literals;

namespace /* anonymous */ {

using ShaderBoolType = std::uint32_t; // #TODO_temp: move to an appropriate place (Shader?)

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
	configuration.allowCPUWrite = false;
	configuration.purpose = graphics::Buffer::Purpose::UNORDERED_ACCESS;
	configuration.size = sizeof(std::uint32_t);

	return graphics::Buffer(graphicsDevice, configuration);
}

graphics::Buffer createCPUResultBuffer(graphics::Device& graphicsDevice) {
	auto configuration = graphics::Buffer::Configuration();
	configuration.allowCPURead = true;
	configuration.allowGPUWrite = true;
	configuration.allowCPUWrite = true;
	configuration.size = sizeof(std::uint32_t);

	return graphics::Buffer(graphicsDevice, configuration);
}

} // anonymous namespace

TextureComparator::TextureComparator(graphics::Device& graphicsDevice) :
	shader_(createComparatorShader(graphicsDevice)),
	resultGPU_(createGPUResultBuffer(graphicsDevice)),
	resultCPU_(createCPUResultBuffer(graphicsDevice)),
	resultUAV_(resultGPU_, graphics::FORMAT_R32_UINT, 0, 1)
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
		const auto lockedResult = commandList.lock(resultCPU_, graphics::CommandList::LockPurpose::WRITE);
		reinterpret_cast<ShaderBoolType&>(*lockedResult.pixels) = true;
	}

	commandList.copy(resultCPU_, resultGPU_);

	commandList.setResource(reference, graphics::ShaderType::COMPUTE, 0);
	commandList.setResource(actual, graphics::ShaderType::COMPUTE, 1);
	commandList.setUnorderedAccessView(resultUAV_, 0);
	commandList.setShader(shader_);

	commandList.dispatch((width + 15) / 16, (height + 15) / 16, 1);

	commandList.copy(resultGPU_, resultCPU_);

	{
		const auto lockedResult = commandList.lock(resultCPU_, graphics::CommandList::LockPurpose::READ);
		return reinterpret_cast<ShaderBoolType&>(*lockedResult.pixels);
	}
}
