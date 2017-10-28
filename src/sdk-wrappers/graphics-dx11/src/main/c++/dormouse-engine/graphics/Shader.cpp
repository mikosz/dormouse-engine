#include "graphics.pch.hpp"

#include "Shader.hpp"

#include "detail/Internals.hpp"
#include "Device.hpp"
#include "DirectXError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

namespace /* anonymous */ {

template <class InternalShaderType>
system::windows::COMWrapper<InternalShaderType> create(Device& device, essentials::ConstBufferView shaderData);

template <>
system::windows::COMWrapper<ID3D11VertexShader> create<ID3D11VertexShader>(
		Device& device, essentials::ConstBufferView shaderData)
{
	system::windows::COMWrapper<ID3D11VertexShader> shader;
	checkDirectXCall(
		detail::Internals::dxDevice(device).CreateVertexShader(
			shaderData.data(), shaderData.size(), nullptr, &shader.get()),
		"Failed to create a vertex shader"
		);
	return shader;
}

template <>
system::windows::COMWrapper<ID3D11GeometryShader> create<ID3D11GeometryShader>(
	Device& device, essentials::ConstBufferView shaderData)
{
	system::windows::COMWrapper<ID3D11GeometryShader> shader;
	checkDirectXCall(
		detail::Internals::dxDevice(device).CreateGeometryShader(
			shaderData.data(), shaderData.size(), nullptr, &shader.get()),
		"Failed to create a geometry shader"
	);
	return shader;
}

template <>
system::windows::COMWrapper<ID3D11HullShader> create<ID3D11HullShader>(
	Device& device, essentials::ConstBufferView shaderData)
{
	system::windows::COMWrapper<ID3D11HullShader> shader;
	checkDirectXCall(
		detail::Internals::dxDevice(device).CreateHullShader(
			shaderData.data(), shaderData.size(), nullptr, &shader.get()),
		"Failed to create a hull shader"
	);
	return shader;
}

template <>
system::windows::COMWrapper<ID3D11DomainShader> create<ID3D11DomainShader>(
	Device& device, essentials::ConstBufferView shaderData)
{
	system::windows::COMWrapper<ID3D11DomainShader> shader;
	checkDirectXCall(
		detail::Internals::dxDevice(device).CreateDomainShader(
			shaderData.data(), shaderData.size(), nullptr, &shader.get()),
		"Failed to create a domain shader"
	);
	return shader;
}

template <>
system::windows::COMWrapper<ID3D11PixelShader> create<ID3D11PixelShader>(
	Device& device, essentials::ConstBufferView shaderData)
{
	system::windows::COMWrapper<ID3D11PixelShader> shader;
	checkDirectXCall(
		detail::Internals::dxDevice(device).CreatePixelShader(
			shaderData.data(), shaderData.size(), nullptr, &shader.get()),
		"Failed to create a pixel shader"
		);
	return shader;
}

template <>
system::windows::COMWrapper<ID3D11ComputeShader> create<ID3D11ComputeShader>(
	Device& device, essentials::ConstBufferView shaderData)
{
	system::windows::COMWrapper<ID3D11ComputeShader> shader;
	checkDirectXCall(
		detail::Internals::dxDevice(device).CreateComputeShader(
			shaderData.data(), shaderData.size(), nullptr, &shader.get()),
		"Failed to create a compute shader"
	);
	return shader;
}

} // anonymous namespace

template <class InternalShaderType>
detail::Shader<InternalShaderType>::Shader(Device& device, essentials::ConstBufferView shaderData) {
	shader_ = create<InternalShaderType>(device, std::move(shaderData));
}

template class detail::Shader<ID3D11VertexShader>;
template class detail::Shader<ID3D11GeometryShader>;
template class detail::Shader<ID3D11HullShader>;
template class detail::Shader<ID3D11DomainShader>;
template class detail::Shader<ID3D11PixelShader>;
template class detail::Shader<ID3D11ComputeShader>;
