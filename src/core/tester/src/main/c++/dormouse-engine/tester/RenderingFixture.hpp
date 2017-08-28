#ifndef _DORMOUSEENGINE_TESTER_RENDERINGFIXTURE_HPP_
#define _DORMOUSEENGINE_TESTER_RENDERINGFIXTURE_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "WindowedFixture.hpp"

namespace dormouse_engine::tester {

class RenderingFixture : public WindowedFixture {
public:

	RenderingFixture();

	graphics::Device& graphicsDevice() {
		return graphicsDevice_;
	}

private:

	graphics::Device graphicsDevice_;

};

} // namespace dormouse_engine::tester

#endif /* _DORMOUSEENGINE_TESTER_RENDERINGFIXTURE_HPP_ */
