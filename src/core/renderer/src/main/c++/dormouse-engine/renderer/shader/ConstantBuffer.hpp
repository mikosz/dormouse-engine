#ifndef _DORMOUSEENGINE_RENDERER_SHADER_CONSTANTBUFFER_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_CONSTANTBUFFER_HPP_

#include <vector>

#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/graphics/ShaderType.hpp"
#include "dormouse-engine/graphics/Buffer.hpp"
#include "../command/commandfwd.hpp"
#include "Parameter.hpp"
#include "Property.hpp"

namespace dormouse_engine::renderer::shader {

class ConstantBuffer {
public:

	using Parameters = std::vector<Parameter>;

	void bind(command::DrawCommand& drawCommand, const Property& root);

private:

	graphics::Buffer buffer_;

	graphics::ShaderType stage_;

	size_t slot_;

	Parameters parameters_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_CONSTANTBUFFER_HPP_ */
