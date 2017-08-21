#ifndef _DORMOUSEENGINE_RENDERER_SHADER_INPUT_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_INPUT_HPP_

#include <vector>

#include "dormouse-engine/milk/graphics/InputLayout.hpp"
#include "Parameter.hpp"

namespace dormouse_engine::renderer {

class Actor;

namespace shader {

class Input {
public:

	using Parameters = std::vector<Parameter>;

	Input(
		milk::graphics::InputLayout layout,
		Parameters perVertexParameters,
		Parameters perInstanceParameters
		) noexcept;

	size_t vertexSize() const noexcept;

	void* writeVertex(void* buffer, const Properties& properties) const;

	size_t instanceSize() const noexcept;

	void* writeInstance(void* buffer, const Properties& properties) const;

	const milk::graphics::InputLayout& layout() const {
		return layout_;
	}

private:

	milk::graphics::InputLayout layout_;

	Parameters perVertexParameters_;

	Parameters perInstanceParameters_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_INPUT_HPP_ */
