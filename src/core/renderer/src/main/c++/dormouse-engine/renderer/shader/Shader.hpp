#ifndef _DORMOUSEENGINE_RENDERER_SHADER_SHADER_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_SHADER_HPP_

#include "dormouse-engine/graphics/CommandList.hpp"
#include "dormouse-engine/graphics/Shader.hpp"

namespace dormouse_engine::renderer::shader {

namespace detail {

template <class GraphicsShaderType>
class Shader {
public:

	void bind(graphics::CommandList& commandList) const {
		commandList.setShader(shader_);
	}

private:

	GraphicsShaderType shader_;

};

} // namespace detail

using VertexShader = detail::Shader<graphics::VertexShader>;
using GeometryShader = detail::Shader<graphics::GeometryShader>;
using HullShader = detail::Shader<graphics::HullShader>;
using DomainShader = detail::Shader<graphics::DomainShader>;
using PixelShader = detail::Shader<graphics::PixelShader>;

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_SHADER_HPP_ */
