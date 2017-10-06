#ifndef _DORMOUSEENGINE_TESTER_RENDERINGFIXTURE_HPP_
#define _DORMOUSEENGINE_TESTER_RENDERINGFIXTURE_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/renderer/control/Viewport.hpp"
#include "WindowedFixture.hpp"

namespace dormouse_engine::tester {

class RenderingFixture : public WindowedFixture {
public:

	RenderingFixture();

	graphics::Device& graphicsDevice() {
		return graphicsDevice_;
	}

	renderer::control::Viewport& fullscreenViewport() {
		return fullscreenViewport_;
	}

	void compareWithReferenceScreen(size_t index) const;

private:

	graphics::Device graphicsDevice_;

	renderer::control::Viewport fullscreenViewport_;

};

} // namespace dormouse_engine::tester

#endif /* _DORMOUSEENGINE_TESTER_RENDERINGFIXTURE_HPP_ */
