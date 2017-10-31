#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_SHADERTYPE_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_SHADERTYPE_HPP_

#include "dormouse-engine/enums.hpp"

#pragma warning(push, 3)
#	include <ponder/pondertype.hpp>
#pragma warning(pop)

struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11PixelShader;
struct ID3D11ComputeShader;

namespace dormouse_engine::graphics {

DE_ENUM(ShaderType,
	(VERTEX)
	(GEOMETRY)
	(HULL)
	(DOMAIN)
	(PIXEL)
	(COMPUTE)
	);

template <class Shader>
constexpr ShaderType shaderTypeFromShader();

template <>
constexpr ShaderType shaderTypeFromShader<ID3D11VertexShader>() {
	return ShaderType::VERTEX;
}

template <>
constexpr ShaderType shaderTypeFromShader<ID3D11GeometryShader>() {
	return ShaderType::GEOMETRY;
}

template <>
constexpr ShaderType shaderTypeFromShader<ID3D11HullShader>() {
	return ShaderType::HULL;
}

template <>
constexpr ShaderType shaderTypeFromShader<ID3D11DomainShader>() {
	return ShaderType::DOMAIN;
}

template <>
constexpr ShaderType shaderTypeFromShader<ID3D11PixelShader>() {
	return ShaderType::PIXEL;
}

template <>
constexpr ShaderType shaderTypeFromShader<ID3D11ComputeShader>() {
	return ShaderType::COMPUTE;
}

namespace detail { void declareShaderType(); }

} // namespace dormouse_engine::graphics

PONDER_AUTO_TYPE(dormouse_engine::graphics::ShaderType, dormouse_engine::graphics::detail::declareShaderType);

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_SHADERTYPE_HPP_ */
