#include "graphics.pch.hpp"

#include "Sampler.hpp"

#include "detail/Internals.hpp"
#include "DirectXError.hpp"
#include "Device.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

Sampler::Sampler(Device& device, const Configuration& configuration) {
	D3D11_SAMPLER_DESC samplerDesc;
	std::memset(&samplerDesc, 0, sizeof(samplerDesc));

	samplerDesc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(configuration.addressModeU);
	samplerDesc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(configuration.addressModeV);
	samplerDesc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(configuration.addressModeW);
	samplerDesc.MinLOD = 0; // TODO: these should be configurable
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MipLODBias = 0;

	samplerDesc.Filter = static_cast<D3D11_FILTER>(configuration.filter);

	checkDirectXCall(
		detail::Internals::dxDevice(device).CreateSamplerState(&samplerDesc, &samplerState_.get()),
		"Failed to create a sampler state"
		);
}
