#ifndef _DORMOUSEENGINE_RENDERER_SHADER_SHADER_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_SHADER_HPP_

#include <vector>

#include "dormouse-engine/graphics/CommandList.hpp"
#include "dormouse-engine/graphics/Shader.hpp"
#include "dormouse-engine/reflection/Object.hpp"
#include "../command/commandfwd.hpp"
#include "PropertyId.hpp"

namespace dormouse_engine::renderer::shader {

class Property;

namespace detail {

class ShaderBase {
protected:

	void doRender(
		command::DrawCommand& cmd,
		const Property& root,
		graphics::ShaderType shaderType
		) const;

private:

	struct Resource {

		PropertyDescriptor descriptor;

		size_t slot;

	};

	using Resources = std::vector<Resource>;

	Resources resources_;

	void bindResource_(
		command::DrawCommand& cmd,
		const Property& root,
		graphics::ShaderType stage,
		const Resource& resource
		) const;

};

template <class GraphicsShaderType>
class Shader : public ShaderBase {
public:

	void bind(graphics::CommandList& commandList) const {
		commandList.setShader(shader_);
	}

	void render(command::DrawCommand& cmd, const Property& root) const {
		doRender(cmd, root, GraphicsShaderType::SHADER_TYPE);
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
