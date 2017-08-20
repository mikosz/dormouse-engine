#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_DEVICE_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_DEVICE_HPP_

#include <functional>
#include <memory>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "dormouse-engine/system/windows/types.hpp"

#include "Texture2d.hpp"
#include "CommandList.hpp"
#include "PrimitiveTopology.hpp"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

namespace dormouse_engine::graphics {

class Device {
public:

	constexpr static auto NDC_NEAR = -1.0f;

	constexpr static auto VECTOR_IS_SINGLE_ROW_MATRIX = true;

	constexpr static auto VECTOR_IS_SINGLE_COLUMN_MATRIX = false;

	using LockedData = std::unique_ptr<void, std::function<void(void*)>>;

	enum class LockPurpose {
		READ = D3D11_MAP_READ,
		WRITE = D3D11_MAP_WRITE,
		READ_WRITE = D3D11_MAP_READ_WRITE,
		WRITE_DISCARD = D3D11_MAP_WRITE_DISCARD,
		WRITE_NO_OVERWRITE = D3D11_MAP_WRITE_NO_OVERWRITE,
	};

	struct Configuration {

		bool debugDevice;

		bool fullscreen;

		bool vsync;

		std::uint32_t sampleCount; // TODO: verify that this is a power of 2

		std::uint32_t sampleQuality;

	};

	Device(system::windows::WindowHandle windowHandle, const Configuration& configuration);

	CommandList& getImmediateCommandList();

	CommandList createDeferredCommandList();

	void beginScene();

	void endScene();

	void submit(CommandList& commandList);

	LockedData lock(Resource& data, LockPurpose lockPurpose);

	Texture2d& backBuffer() {
		return backBuffer_;
	}

	Texture2d& depthStencil() {
		return depthStencil_;
	}

	ID3D11Device& internalDevice() {
		return *d3dDevice_;
	}

private:

	Configuration configuration_;

	system::windows::COMWrapper<IDXGIAdapter> adapter_;

	system::windows::COMWrapper<ID3D11Device> d3dDevice_;

	CommandList immediateCommandList_;

	system::windows::COMWrapper<IDXGISwapChain> swapChain_;

	Texture2d backBuffer_;

	Texture2d depthStencil_;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_DEVICE_HPP_ */
