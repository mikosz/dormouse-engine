#ifndef _DORMOUSEENGINE_GRAPHICS_SHADER_HPP_
#define _DORMOUSEENGINE_GRAPHICS_SHADER_HPP_

#include <memory>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/graphics/ShaderType.hpp"
#include "dormouse-engine/system/windows/COMWrapper.hpp"

namespace dormouse_engine::graphics {

class Device;

namespace detail {

template <class InternalShaderType>
class Shader {
public:

	static const auto SHADER_TYPE = shaderTypeFromShader<InternalShaderType>();

	Shader(Device& renderer, const void* data, size_t size);

	InternalShaderType& internalShader() {
		return *shader_;
	}

private:

	system::windows::COMWrapper<InternalShaderType> shader_;

};

} // namespace detail

using VertexShader = detail::Shader<ID3D11VertexShader>;
using GeometryShader = detail::Shader<ID3D11GeometryShader>;
using HullShader = detail::Shader<ID3D11HullShader>;
using DomainShader = detail::Shader<ID3D11DomainShader>;
using PixelShader = detail::Shader<ID3D11PixelShader>;

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_SHADER_HPP_ */
