#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_RENDERSTATE_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_RENDERSTATE_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "detail/detailfwd.hpp"
#include "dormouse-engine/enums.hpp"
#include "dormouse-engine/system/windows/COMWrapper.hpp"

namespace dormouse_engine::graphics {

class Device;

class RenderState {
public:

	DE_MEMBER_ENUM_VALUES(
		CullMode,
		(BACK)(D3D11_CULL_BACK)
		(FRONT)(D3D11_CULL_FRONT)
		(NONE)(D3D11_CULL_NONE)
		);

	DE_MEMBER_ENUM_VALUES(
		FillMode,
		(SOLID)(D3D11_FILL_SOLID)
		(WIREFRAME)(D3D11_FILL_WIREFRAME)
		);

	struct Configuration {

		CullMode cullMode = CullMode::BACK;

		FillMode fillMode = FillMode::SOLID;

		bool frontCounterClockwise = false;

		bool blendingEnabled = false;

	};

	RenderState() = default;

	RenderState(Device& device, const Configuration& configuration);

private:

	system::windows::COMWrapper<ID3D11RasterizerState> rasteriserState_;

	system::windows::COMWrapper<ID3D11BlendState> blendState_;

	friend struct detail::Internals;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_RENDERSTATE_HPP_ */
