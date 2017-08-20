#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_TEXTURE_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_TEXTURE_HPP_

#include <type_traits>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/enums/Mask.hpp"
#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "PixelFormat.hpp"
#include "Resource.hpp"

namespace dormouse_engine::graphics {

class Device;

class Texture : public Resource {
public:

	DE_MEMBER_FLAG_VALUES(
		CreationPurpose,
		(SHADER_RESOURCE)(D3D11_BIND_SHADER_RESOURCE)
		(RENDER_TARGET)(D3D11_BIND_RENDER_TARGET)
		(DEPTH_STENCIL)(D3D11_BIND_DEPTH_STENCIL)
		);

	void initialise(Device& renderer, dormouse_engine::Mask<CreationPurpose> purposeFlags);

	void reset();

	ID3D11RenderTargetView& internalRenderTargetView() {
		return *renderTargetView_;
	}

	ID3D11DepthStencilView& internalDepthStencilView() {
		return *depthStencilView_;
	}

private:

	system::windows::COMWrapper<ID3D11RenderTargetView> renderTargetView_;

	system::windows::COMWrapper<ID3D11DepthStencilView> depthStencilView_;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_TEXTURE_HPP_ */
