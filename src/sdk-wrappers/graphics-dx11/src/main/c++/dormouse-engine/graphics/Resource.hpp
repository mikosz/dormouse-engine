#ifndef _DORMOUSEENGINE_GRAPHICS_RESOURCE_HPP_
#define _DORMOUSEENGINE_GRAPHICS_RESOURCE_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"

namespace dormouse_engine::graphics {

const auto RESOURCE_SLOT_COUNT_PER_SHADER = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;

class Resource {
public:

	Resource() = default;

	void reset() {
		resource_.reset();
		shaderResourceView_.reset();
	}

	system::windows::COMWrapper<ID3D11Resource> internalResource() const { // TODO: move to detail interface
		return resource_;
	}

	system::windows::COMWrapper<ID3D11ShaderResourceView> internalShaderResourceView() const {
		return shaderResourceView_;
	}

protected:

	system::windows::COMWrapper<ID3D11Resource> resource_;

	system::windows::COMWrapper<ID3D11ShaderResourceView> shaderResourceView_;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_RESOURCE_HPP_ */
