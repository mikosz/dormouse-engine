#ifndef _DORMOUSEENGINE_TESTER_RENDERINGFIXTURE_HPP_
#define _DORMOUSEENGINE_TESTER_RENDERINGFIXTURE_HPP_

#include "App.hpp"

namespace dormouse_engine::tester {

class EngineTesterFixture : public WindowedFixture {
public:

	EngineTesterFixture();

	graphics::Device& graphicsDevice() {
		return graphicsDevice_;
	}

	renderer::control::Viewport& fullscreenViewport() {
		return fullscreenViewport_;
	}

	void compareWithReferenceScreen(size_t index);

	App& testerApp();

};

} // namespace dormouse_engine::tester

#endif /* _DORMOUSEENGINE_TESTER_RENDERINGFIXTURE_HPP_ */
