#ifndef _DORMOUSEENGINE_RENDERER_SHADER_CONSTANTBUFFER_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_CONSTANTBUFFER_HPP_

#include <vector>

#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/ShaderType.hpp"
#include "dormouse-engine/graphics/Buffer.hpp"
#include "../command/commandfwd.hpp"
#include "Parameter.hpp"

namespace dormouse_engine::renderer::shader {

class Property;

class ConstantBuffer {
public:

	using Parameters = std::vector<Parameter>;

	ConstantBuffer(
		graphics::Device& graphicsDevice,
		graphics::ShaderType stage,
		size_t slot,
		size_t size,
		Parameters parameters
		);

	void bind(command::DrawCommand& drawCommand, const Property& root) const;

private:

	graphics::Buffer buffer_;

	graphics::ShaderType stage_;

	size_t slot_;

	size_t size_;

	Parameters parameters_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_CONSTANTBUFFER_HPP_ */
