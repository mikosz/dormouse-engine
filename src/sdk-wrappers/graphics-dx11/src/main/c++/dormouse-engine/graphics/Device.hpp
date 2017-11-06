#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_DEVICE_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_DEVICE_HPP_

#include <functional>
#include <memory>
#include <vector>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "dormouse-engine/system/windows/types.hpp"
#include "detail/detailfwd.hpp"
#include "Texture.hpp"
#include "CommandList.hpp"
#include "PrimitiveTopology.hpp"

#pragma comment(lib, "d3d11.lib")

namespace dormouse_engine::graphics {

class Adapter;

class Device {
public:

	constexpr static auto NDC_NEAR = -1.0f;

	constexpr static auto VECTOR_IS_SINGLE_ROW_MATRIX = false;

	constexpr static auto VECTOR_IS_SINGLE_COLUMN_MATRIX = !VECTOR_IS_SINGLE_ROW_MATRIX;

	using DeviceDestroyedHandler = std::function<void()>;

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

	Device(const Adapter& adapter, bool debug);

	~Device();

	void addDeviceDestroyedHandler(DeviceDestroyedHandler handler) {
		deviceDestroyedHandlers_.emplace_back(std::move(handler));
	}

	CommandList& getImmediateCommandList();

	CommandList createDeferredCommandList();

	// TODO: duplicated with CommandList::lock, remove
	LockedData lock(Resource& data, LockPurpose lockPurpose);

private:

	system::windows::COMWrapper<ID3D11Device> d3dDevice_;

	std::vector<DeviceDestroyedHandler> deviceDestroyedHandlers_;

	CommandList immediateCommandList_;

	friend struct detail::Internals;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_DEVICE_HPP_ */
