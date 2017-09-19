#include "InputLayout.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer::shader;

namespace /*anonymous */ {

graphics::PixelFormat::DataType elementDataType(
	graphics::ShaderReflection::InputParameterInfo::DataType inputDataType)
{
	switch (inputDataType) {
	case graphics::ShaderReflection::InputParameterInfo::DataType::FLOAT:
		return graphics::PixelFormat::DataType::FLOAT;
	case graphics::ShaderReflection::InputParameterInfo::DataType::INT:
		return graphics::PixelFormat::DataType::SINT;
	case graphics::ShaderReflection::InputParameterInfo::DataType::UINT:
		return graphics::PixelFormat::DataType::UINT;
	}

	assert(!"Invalid input data type");
	return graphics::PixelFormat::DataType::UNKNOWN;
}

graphics::InputLayout createInputLayout(
	graphics::Device& graphicsDevice,
	essentials::ConstBufferView compiledVertexShaderObjectData
	)
{
	auto elements = graphics::InputLayout::Elements();
	const auto reflectionData = graphics::ShaderReflection( // TODO: ShaderReflection constructor should accept ConstBufferView
		compiledVertexShaderObjectData.data(), compiledVertexShaderObjectData.size());

	elements.reserve(reflectionData.inputParameters().size());

	for (const auto& inputParameter : reflectionData.inputParameters()) {
		auto format = graphics::PixelFormat();

		const auto dataType = elementDataType(inputParameter.dataType);

		assert(inputParameter.elements <= 4u);
		if (inputParameter.elements > 0u) {
			format = format <<
				graphics::PixelFormat::Channel(graphics::PixelFormat::ChannelType::X, dataType, 32u); // TODO: 32u?
		}
		if (inputParameter.elements > 1u) {
			format = format <<
				graphics::PixelFormat::Channel(graphics::PixelFormat::ChannelType::Y, dataType, 32u);
		}
		if (inputParameter.elements > 2u) {
			format = format <<
				graphics::PixelFormat::Channel(graphics::PixelFormat::ChannelType::Z, dataType, 32u);
		}
		if (inputParameter.elements > 3u) {
			format = format <<
				graphics::PixelFormat::Channel(graphics::PixelFormat::ChannelType::W, dataType, 32u);
		}

		elements.emplace_back(
			inputParameter.semantic,
			inputParameter.semanticIndex,
			format,
			graphics::InputLayout::SlotType::PER_VERTEX_DATA, // TODO: temp
			0u
			);
	}

	return graphics::InputLayout(graphicsDevice, std::move(elements));
}

} // anonymous namespace

InputLayout::InputLayout(
	graphics::Device& graphicsDevice,
	essentials::ConstBufferView compiledVertexShaderObjectData
	) :
	inputLayout_(createInputLayout(graphicsDevice, compiledVertexShaderObjectData))
{
}
