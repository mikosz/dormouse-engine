#include "App.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::engine::app;

App::App(const wm::MainArguments& mainArguments, const wm::Window::Configuration& mainWindowConfiguration) :
	wmApp_(mainArguments),
	mainWindow_(mainWindowConfiguration, essentials::make_observer(&wmApp_))
{
}

void App::run() {
	while (!wmApp_.closeRequested()) {
		update();
	}
}

void App::update() {
	clock_.tick();
	wmApp_.update();
}
