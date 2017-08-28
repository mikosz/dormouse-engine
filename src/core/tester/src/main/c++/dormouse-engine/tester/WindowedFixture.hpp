#ifndef _DORMOUSEENGINE_TESTER_WINDOWEDFIXTURE_HPP_
#define _DORMOUSEENGINE_TESTER_WINDOWEDFIXTURE_HPP_

#include "dormouse-engine/wm/App.hpp"
#include "dormouse-engine/wm/Window.hpp"

namespace dormouse_engine::tester {

class WindowedFixture {
public:

	WindowedFixture();

	wm::App& app() {
		return *app_;
	}

	wm::Window& window() {
		return window_;
	}

private:

	std::shared_ptr<wm::App> app_;

	wm::Window window_;

};

} // namespace dormouse_engine::tester

#endif /* _DORMOUSEENGINE_TESTER_WINDOWEDFIXTURE_HPP_ */
