#ifndef _DORMOUSEENGINE_RENDERER_SHADER_SHADER_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_SHADER_HPP_

#include <vector>
#include <unordered_map>

#include "dormouse-engine/graphics/Shader.hpp"

#include "dormouse-engine/essentials/smart-pointer-definitions.hpp"

#include "dormouse-engine/mesh/MaterialConfiguration.hpp"

#include "Resource.hpp"
#include "ConstantBuffer.hpp"

namespace dormouse_engine::renderer {

class PassContext;
class Actor;
class Scene;
class DrawCommand;

namespace shader {

class UnknownShader {
public:

	using ConstantBuffers = std::vector<ConstantBuffer>;

	using Resources = std::vector<Resource>;

	virtual ~UnknownShader() = default;

};

namespace detail {

template <class GraphicsShaderType>
class Shader : public UnknownShader {
public:

	Shader(
		GraphicsShaderType shaderData,
		ConstantBuffers constantBuffers,
		Resources resources
		) :
		shaderData_(std::move(shaderData)),
		constantBuffers_(std::move(constantBuffers)),
		resources_(std::move(resources))
	{
	}

	void bind(DrawCommand& drawCommand, const Properties& properties) {
		for (auto& buffer : constantBuffers_) {
			buffer.bind(drawCommand, properties);
		}

		for (auto resource : resources_) {
			resource.bind(drawCommand, properties);
		}
	}

	GraphicsShaderType& shaderData() {
		return shaderData_;
	}

private:

	GraphicsShaderType shaderData_;

	ConstantBuffers constantBuffers_;

	Resources resources_;

};

} // namespace detail

using VertexShader = detail::Shader<graphics::VertexShader>;
using GeometryShader = detail::Shader<graphics::GeometryShader>;
using HullShader = detail::Shader<graphics::HullShader>;
using DomainShader = detail::Shader<graphics::DomainShader>;
using PixelShader = detail::Shader<graphics::PixelShader>;

DE_SMART_POINTER_DEFINITONS(VertexShader);
DE_SMART_POINTER_DEFINITONS(GeometryShader);
DE_SMART_POINTER_DEFINITONS(HullShader);
DE_SMART_POINTER_DEFINITONS(DomainShader);
DE_SMART_POINTER_DEFINITONS(PixelShader);

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_SHADER_HPP_ */
