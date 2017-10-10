#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_SAMPLER_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_SAMPLER_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/enums.hpp"
#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "detail/detailfwd.hpp"

namespace dormouse_engine::graphics {

class Device;

const auto SAMPLER_SLOT_COUNT_PER_SHADER = D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT;

class Sampler {
public:

	DE_MEMBER_ENUM_VALUES(
		AddressMode,
		(CLAMP)(D3D11_TEXTURE_ADDRESS_CLAMP)
		(WRAP)(D3D11_TEXTURE_ADDRESS_WRAP)
		(MIRROR)(D3D11_TEXTURE_ADDRESS_MIRROR)
		(MIRROR_ONCE)(D3D11_TEXTURE_ADDRESS_MIRROR_ONCE)
		);

	DE_MEMBER_ENUM_VALUES(
		Filter,
		(MIN_MAG_MIP_POINT)(D3D11_FILTER_MIN_MAG_MIP_POINT)
		(MIN_MAG_MIP_LINEAR)(D3D11_FILTER_MIN_MAG_MIP_LINEAR)
		(MIN_MAG_LINEAR_MIP_POINT)(D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT)
		(ANISOTROPIC)(D3D11_FILTER_ANISOTROPIC)
		);

	struct Configuration {

		AddressMode addressModeU;

		AddressMode addressModeV;

		AddressMode addressModeW;

		Filter filter;

	};

	Sampler() {
	}

	Sampler(Device& renderer, const Configuration& configuration);

private:

	system::windows::COMWrapper<ID3D11SamplerState> samplerState_;

	friend struct detail::Internals;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_SAMPLER_HPP_ */
