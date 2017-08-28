#include "WindowedFixture.hpp"

#include <cassert>

using namespace dormouse_engine;
using namespace dormouse_engine::tester;

namespace /* anonymous */ {

wm::MainArguments mainArguments() {
	auto globalMainArguments = wm::GlobalMainArguments::instance();
	assert(globalMainArguments);
	return *globalMainArguments;
}

wm::Window::Configuration windowConfiguration() {
	auto result = wm::Window::Configuration();

	result.className = "RenderingFixtureWindow";
	result.title = "Rendering fixture window";
	result.fullscreen = false;
	result.width = 800u;
	result.height = 600u;

	return result;
}

} // anonymous namespace

WindowedFixture::WindowedFixture() :
	app_(std::make_shared<wm::App>(mainArguments())),
	window_(windowConfiguration(), app_)
{
}
