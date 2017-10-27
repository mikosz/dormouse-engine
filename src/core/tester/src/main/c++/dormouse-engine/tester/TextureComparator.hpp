#ifndef _DORMOUSEENGINE_TESTER_TEXTURECOMPARATOR_HPP_
#define _DORMOUSEENGINE_TESTER_TEXTURECOMPARATOR_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/renderer/control/ResourceView.hpp"

namespace dormouse_engine::tester {

class TextureComparator {
public:

	TextureComparator(
		graphics::Device& graphicsDevice,
		renderer::control::ResourceView reference,
		renderer::control::ResourceView actual
		);

private:

	renderer::control::ResourceView reference_;

	renderer::control::ResourceView actual_;

};

} // namespace dormouse_engine::tester

#endif /* _DORMOUSEENGINE_TESTER_TEXTURECOMPARATOR_HPP_ */
