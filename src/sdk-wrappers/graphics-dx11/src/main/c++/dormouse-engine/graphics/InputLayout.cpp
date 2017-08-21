#include "InputLayout.hpp"

#include <iterator>

#include "dormouse-engine/exceptions/LogicError.hpp"
#include "dormouse-engine/essentials/Range.hpp"
#include "DirectXError.hpp"
#include "Device.hpp"
#include "ShaderCompiler.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

using namespace std::string_literals;

namespace /* anonymous */ {

size_t inputSlotIndex(InputLayout::SlotType inputSlot) {
	switch (inputSlot) {
	case InputLayout::SlotType::PER_VERTEX_DATA:
		return 0u;
	case InputLayout::SlotType::PER_INSTANCE_DATA:
		return 1u;
	default:
		assert(!"Unexpected input slot");
		return 0u;
	}
}

system::windows::COMWrapper<ID3D11InputLayout> createLayout(
	const InputLayout::Elements& elements,
	Device& renderer,
	std::vector<std::uint8_t> shaderData
	)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
	descs.reserve(elements.size());

	for (const auto& element : elements) {
		descs.emplace_back();
		auto& desc = descs.back();

		std::memset(&desc, 0, sizeof(desc));

		desc.SemanticName = element.semantic.c_str();
		desc.SemanticIndex = static_cast<UINT>(element.semanticIndex);
		desc.Format = static_cast<DXGI_FORMAT>(element.format.id());
		desc.InputSlot = static_cast<UINT>(inputSlotIndex(element.inputSlotType));
		desc.InputSlotClass = static_cast<D3D11_INPUT_CLASSIFICATION>(element.inputSlotType);
		desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		desc.InstanceDataStepRate = static_cast<UINT>(element.instanceDataStepRate);
	}

	const auto dummy = D3D11_INPUT_ELEMENT_DESC();

	system::windows::COMWrapper<ID3D11InputLayout> layout;
	checkDirectXCall(
		renderer.internalDevice().CreateInputLayout(
			descs.empty() ? &dummy : descs.data(),
			static_cast<UINT>(descs.size()),
			shaderData.data(),
			shaderData.size(),
			&layout.get()
			),
		"Failed to create an input layout"
		);

	return layout;
}

std::string formatHLSLType(const PixelFormat& format) {
	const auto channelsUsed = format.channelsUsed();
	if (channelsUsed == 0) {
		throw exceptions::LogicError("Can't provide a HLSL type for empty pixel format");
	}

	const auto channelType = format.channel(0).dataType;

	for (auto channelIndex : essentials::range<size_t>(0, channelsUsed)) {
		if (channelType != format.channel(channelIndex).dataType) {
			throw exceptions::LogicError("Can't provide a HLSL type for non-uniform pixel format");
		}
	}

	auto result = std::string();

	switch (channelType) {
	case PixelFormat::DataType::FLOAT:
		result = "float";
		break;
	default:
		throw exceptions::LogicError("Can't provide a HLSL type for data type "s + toString(channelType));
	}

	result += std::to_string(channelsUsed);

	return result;
}

std::vector<std::uint8_t> createDummyVertexShader(const InputLayout::Elements& elements) {
	const auto* const prefix = "struct VIn {\n";
	const auto* const suffix = 
		"};\n"
		"\n"
		"float4 main(VIn vin) : SV_POSITION { return (0.0f).xxxx; }\n"
		;

	std::ostringstream shaderTextStream;
	shaderTextStream << prefix;

	for (const auto& element : elements) {
		shaderTextStream
			<< "\t"
			<< formatHLSLType(element.format) << " "
			<< element.semantic << "_" << element.semanticIndex
			<< " : "
			<< element.semantic
			<< ";\n";
	}

	shaderTextStream << suffix;

	std::vector<std::uint8_t> shaderData;
	const auto shaderText = shaderTextStream.str();
	shaderData.reserve(shaderText.size() + 1);
	std::copy(shaderText.begin(), shaderText.end(), std::back_inserter(shaderData));
	shaderData.emplace_back('\0');

	return ShaderCompiler().compile(
        essentials::viewBuffer(shaderData),
		"-- input layout generated --",
		"main",
		ShaderType::VERTEX,
		ShaderCompiler::IncludeHandler()
		);
}

} // anonymous namespace

InputLayout::Element::Element(
	std::string semantic,
	size_t semanticIndex,
	PixelFormat format,
	SlotType inputSlotType,
	size_t instanceDataStepRate
	) :
	semantic(std::move(semantic)),
	semanticIndex(semanticIndex),
	format(format),
	inputSlotType(inputSlotType),
	instanceDataStepRate(instanceDataStepRate)
{
}

InputLayout::InputLayout(
	Device& renderer,
	const Elements& elements
	) :
	dxInputLayout_(createLayout(elements, renderer, createDummyVertexShader(elements)))
{
}
