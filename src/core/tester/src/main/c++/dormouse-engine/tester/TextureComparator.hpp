#ifndef _DORMOUSEENGINE_TESTER_TEXTURECOMPARATOR_HPP_
#define _DORMOUSEENGINE_TESTER_TEXTURECOMPARATOR_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Shader.hpp"
#include "dormouse-engine/graphics/Resource.hpp"
#include "dormouse-engine/graphics/Buffer.hpp"

namespace dormouse_engine::tester {

class TextureComparator {
public:

	TextureComparator(graphics::Device& graphicsDevice);

	bool compare(
		graphics::Device& graphicsDevice,
		const graphics::ResourceView& reference,
		const graphics::ResourceView& actual,
		size_t width,
		size_t height
		) const;

private:

	graphics::ComputeShader shader_;

	graphics::Buffer resultGPU_;

	graphics::Buffer resultCPU_;

	graphics::UnorderedAccessView resultUAV_;

};

} // namespace dormouse_engine::tester

#endif /* _DORMOUSEENGINE_TESTER_TEXTURECOMPARATOR_HPP_ */
