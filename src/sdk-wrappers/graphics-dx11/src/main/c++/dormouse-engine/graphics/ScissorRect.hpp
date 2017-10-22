#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_SCISSORRECT_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_SCISSORRECT_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "detail/detailfwd.hpp"

namespace dormouse_engine::graphics {

class ScissorRect {
public:

	struct Configuration {

		int top;

		int bottom;

		int left;

		int right;

	};

	ScissorRect() = default;

	ScissorRect(const Configuration& configuration);

private:

	bool enabled_ = false;

	D3D11_RECT scissorRect_;

	friend struct detail::Internals;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_SCISSORRECT_HPP_ */
