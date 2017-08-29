#ifndef _DORMOUSEENGINE_GRAPHICS_RESOURCE_HPP_
#define _DORMOUSEENGINE_GRAPHICS_RESOURCE_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "PixelFormat.hpp"

namespace dormouse_engine::graphics {

class Device;
class Buffer;
class Texture;

const auto RESOURCE_SLOT_COUNT_PER_SHADER = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;

class Resource {
public:

	Resource() = default;

	system::windows::COMWrapper<ID3D11Resource> internalResource() const { // TODO: move to detail interface
		return resource_;
	}

protected:

	system::windows::COMWrapper<ID3D11Resource> resource_;

	Resource(system::windows::COMWrapper<ID3D11Resource> resource) :
		resource_(std::move(resource))
	{
	}

};

class ResourceView {
public:

	ResourceView(Device& device, const Texture& texture);

	ResourceView(Device& device, const Buffer& buffer, PixelFormat elementFormat);

	system::windows::COMWrapper<ID3D11ShaderResourceView> internalResourceView() const {
		return resourceView_;
	}

private:

	system::windows::COMWrapper<ID3D11ShaderResourceView> resourceView_;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_RESOURCE_HPP_ */
