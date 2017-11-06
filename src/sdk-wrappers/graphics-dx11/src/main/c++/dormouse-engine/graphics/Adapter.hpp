#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_ADAPTER_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_ADAPTER_HPP_

#include <vector>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "detail/detailfwd.hpp"

#pragma comment(lib, "dxgi.lib")

namespace dormouse_engine {
namespace graphics {

class Adapter {
public:

	static std::vector<Adapter> create();

	Adapter() = default;

private:

	Adapter(system::windows::COMWrapper<IDXGIAdapter> adapter) :
		adapter_(std::move(adapter))
	{
	}

	system::windows::COMWrapper<IDXGIAdapter> adapter_;

	friend struct detail::Internals;

};

} // namespace graphics
} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_ADAPTER_HPP_ */
