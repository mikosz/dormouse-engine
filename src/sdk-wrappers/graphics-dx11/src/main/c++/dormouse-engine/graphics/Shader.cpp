#include "graphics.pch.hpp"

#include "Shader.hpp"

#include "detail/Internals.hpp"
#include "Device.hpp"
#include "DirectXError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

namespace /* anonymous */ {

template <class InternalShaderType>
system::windows::COMWrapper<InternalShaderType> create(Device& device, void* data, size_t size);

template <>
system::windows::COMWrapper<ID3D11VertexShader> create<ID3D11VertexShader>(
		Device& device, void* data, size_t size) {
	system::windows::COMWrapper<ID3D11VertexShader> shader;
	checkDirectXCall(
		detail::Internals::dxDevice(device).CreateVertexShader(data, size, 0, &shader.get()),
		"Failed to create a vertex shader"
		);
	return shader;
}

template <>
system::windows::COMWrapper<ID3D11GeometryShader> create<ID3D11GeometryShader>(
	Device& device, void* data, size_t size) {
	system::windows::COMWrapper<ID3D11GeometryShader> shader;
	checkDirectXCall(
		detail::Internals::dxDevice(device).CreateGeometryShader(data, size, 0, &shader.get()),
		"Failed to create a geometry shader"
	);
	return shader;
}

template <>
system::windows::COMWrapper<ID3D11HullShader> create<ID3D11HullShader>(
	Device& device, void* data, size_t size) {
	system::windows::COMWrapper<ID3D11HullShader> shader;
	checkDirectXCall(
		detail::Internals::dxDevice(device).CreateHullShader(data, size, 0, &shader.get()),
		"Failed to create a hull shader"
	);
	return shader;
}

template <>
system::windows::COMWrapper<ID3D11DomainShader> create<ID3D11DomainShader>(
	Device& device, void* data, size_t size) {
	system::windows::COMWrapper<ID3D11DomainShader> shader;
	checkDirectXCall(
		detail::Internals::dxDevice(device).CreateDomainShader(data, size, 0, &shader.get()),
		"Failed to create a domain shader"
	);
	return shader;
}

template <>
system::windows::COMWrapper<ID3D11PixelShader> create<ID3D11PixelShader>(
	Device& device, void* data, size_t size) {
	system::windows::COMWrapper<ID3D11PixelShader> shader;
	checkDirectXCall(
		detail::Internals::dxDevice(device).CreatePixelShader(data, size, 0, &shader.get()),
		"Failed to create a vertex shader"
		);
	return shader;
}

} // anonymous namespace

template <class InternalShaderType>
detail::Shader<InternalShaderType>::Shader(Device& device, const void* data, size_t size) {
	shader_ = create<InternalShaderType>(device, const_cast<void*>(data), size);
}

template class detail::Shader<ID3D11VertexShader>;
template class detail::Shader<ID3D11GeometryShader>;
template class detail::Shader<ID3D11HullShader>;
template class detail::Shader<ID3D11DomainShader>;
template class detail::Shader<ID3D11PixelShader>;
