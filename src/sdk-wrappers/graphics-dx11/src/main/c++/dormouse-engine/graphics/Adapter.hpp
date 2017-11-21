#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_ADAPTER_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_ADAPTER_HPP_

#include <vector>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "detail/detailfwd.hpp"
#include "PixelFormat.hpp"

#pragma comment(lib, "dxgi.lib")

namespace dormouse_engine {
namespace graphics {

struct DisplayMode {
	size_t width = 0;
	size_t height = 0;
	size_t refreshRateNumerator = 0;
	size_t refreshRateDenominator = 0;
	PixelFormat pixelFormat;
};

class Output {
public:

	Output() = default;

	std::vector<DisplayMode> displayModes(PixelFormat pixelFormat) const;

private:

	Output(system::windows::COMWrapper<IDXGIOutput> output) :
		output_(output)
	{
	}

	system::windows::COMWrapper<IDXGIOutput> output_;

	friend class Adapter;

};

class Adapter {
public:

	static std::vector<Adapter> create();

	Adapter() = default;

	std::vector<Output> outputs() const;

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
