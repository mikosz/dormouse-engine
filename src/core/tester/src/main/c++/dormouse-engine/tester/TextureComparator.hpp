#ifndef _DORMOUSEENGINE_TESTER_TEXTURECOMPARATOR_HPP_
#define _DORMOUSEENGINE_TESTER_TEXTURECOMPARATOR_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Shader.hpp"
#include "dormouse-engine/renderer/control/ResourceView.hpp"

namespace dormouse_engine::tester {

class TextureComparator {
public:

	TextureComparator(graphics::Device& graphicsDevice);

	void compare(
		renderer::control::ResourceView reference,
		renderer::control::ResourceView actual
		) const;

private:

	graphics::ComputeShader shader_;

};

} // namespace dormouse_engine::tester

#endif /* _DORMOUSEENGINE_TESTER_TEXTURECOMPARATOR_HPP_ */
