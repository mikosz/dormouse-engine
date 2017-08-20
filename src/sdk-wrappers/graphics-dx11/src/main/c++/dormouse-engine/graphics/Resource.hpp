#ifndef _DORMOUSEENGINE_GRAPHICS_RESOURCE_HPP_
#define _DORMOUSEENGINE_GRAPHICS_RESOURCE_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"

namespace dormouse_engine {
namespace graphics {

class Resource {
public:

	Resource() = default;

	virtual ~Resource() = default;

	void reset() {
		resource_.reset();
	}

	system::windows::COMWrapper<ID3D11Resource> internalResource() const { // TODO: move to detail interface
		return resource_;
	}

	system::windows::COMWrapper<ID3D11ShaderResourceView> internalShaderResourceView() const {
		return shaderResourceView_;
	}

protected:

	// TODO: is this how it's supposed to be done?
	Resource(const Resource&) = default;

	Resource(Resource&&) = default;

	Resource& operator=(const Resource&) = default;

	Resource& operator=(Resource&&) = default;

	system::windows::COMWrapper<ID3D11Resource> resource_;

	system::windows::COMWrapper<ID3D11ShaderResourceView> shaderResourceView_;

};

} // namespace graphics
} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_GRAPHICS_RESOURCE_HPP_ */
