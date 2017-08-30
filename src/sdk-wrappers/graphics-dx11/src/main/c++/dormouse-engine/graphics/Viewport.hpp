#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_VIEWPORT_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_VIEWPORT_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "detail/detailfwd.hpp"

namespace dormouse_engine::graphics {

class Viewport {
public:

	struct Configuration {

		float width;

		float height;

		float minDepth;

		float maxDepth;

		float topLeftX;

		float topLeftY;

	};

	Viewport(const Configuration& configuration);

private:

	D3D11_VIEWPORT viewport_;

	friend struct detail::Internals;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_VIEWPORT_HPP_ */
