#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_TEXTURE1D_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_TEXTURE1D_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "PixelFormat.hpp"
#include "Texture.hpp"

namespace dormouse_engine {
namespace graphics {

class Renderer;
class Image;

class Texture1d : public Texture {
public:

	struct Configuration {

		size_t width;

		size_t arraySize;

		size_t mipLevels;

		PixelFormat pixelFormat;

		bool allowModifications;

		bool allowCPURead;

		bool allowGPUWrite;

		dormouse_engine::Mask<CreationPurpose> purposeFlags;

		const void* initialData;

		Configuration() {
			std::memset(this, 0, sizeof(decltype(*this))); // TODO: TEMP TEMP TEMP
			mipLevels = 1;
			arraySize = 1;
		}

	};

	Texture1d() {
	}

	Texture1d(Renderer& renderer, const Configuration& configuration);

	Texture1d(Renderer& renderer, const Image& image);

	Texture1d(system::windows::COMWrapper<ID3D11Texture1D> texture)
	{
		resource_.reset(texture.get());
	}

	void initialise(Renderer& renderer, const Configuration& configuration);

	void initialise(
		Renderer& renderer,
		dormouse_engine::Mask<CreationPurpose> purposeFlags,
		system::windows::COMWrapper<ID3D11Texture1D> texture
	);

};

} // namespace graphics
} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_TEXTURE1D_HPP_ */
