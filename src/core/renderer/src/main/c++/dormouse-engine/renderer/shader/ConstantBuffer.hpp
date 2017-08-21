#ifndef _DORMOUSEENGINE_RENDERER_SHADER_CONSTANTBUFFER_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_CONSTANTBUFFER_HPP_

#include <cassert>
#include <memory>
#include <vector>
#include <numeric>

#include "dormouse-engine/exceptions/LogicError.hpp"

#include "dormouse-engine/graphics/ConstantBuffer.hpp"

#include "../DrawCommand.hpp"
#include "Parameter.hpp"

namespace dormouse_engine::renderer::shader {

class ConstantBuffer {
public:

	using Parameters = std::vector<Parameter>;

	ConstantBuffer(
		graphics::Device& renderer,
		graphics::ShaderType shaderType,
		size_t size,
		size_t slot,
		Parameters parameters
		) :
		stage_(shaderType),
		slot_(slot),
		buffer_(
			renderer,
			graphics::Buffer::Configuration(
				size,
				0,
				true,
				false,
				false
				)
			),
		data_(size),
		parameters_(std::move(parameters))
	{
	}

	void bind(DrawCommand& drawCommand, const Properties& properties) {
		for (const auto parameter : parameters_) {
			auto* endPtr = parameter.write(data_.data(), properties);
			if (endPtr > (&data_.back() + 1)) {
				throw coconut_tools::exceptions::LogicError("Data written past end of buffer");
			}
		}

		// TODO: unnecessarily keeping data in data_ AND in DrawCommand
		drawCommand.addConstantBufferData(&buffer_, data_.data(), data_.size(), stage_, slot_);
	}

private:

	graphics::ShaderType stage_;

	size_t slot_;

	graphics::ConstantBuffer buffer_;

	std::vector<std::uint8_t> data_;

	Parameters parameters_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_CONSTANTBUFFER_HPP_ */
