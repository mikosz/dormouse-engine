#ifndef _DORMOUSEENGINE_RENDERER_SHADER_INPUTLAYOUT_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_INPUTLAYOUT_HPP_

#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/InputLayout.hpp"
#include "Shader.hpp"

namespace dormouse_engine::renderer::shader {

class InputLayout {
public:

	InputLayout() = default;

	InputLayout(
		graphics::Device& graphicsDevice,
		essentials::ConstBufferView compiledVertexShaderObjectData
		);

	InputLayout(
		graphics::Device& graphicsDevice,
		const graphics::InputLayout::Elements& elements
		);

	void bind(graphics::CommandList& commandList) const {
		commandList.setInputLayout(inputLayout_);
	}

private:

	graphics::InputLayout inputLayout_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_INPUTLAYOUT_HPP_ */
