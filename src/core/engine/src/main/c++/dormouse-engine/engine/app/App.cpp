#include "App.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::engine::app;

namespace /* anonymous */ {

graphics::Device::Configuration graphicsDeviceConfiguration() {
	auto configuration = graphics::Device::Configuration();

	// TODO: ...

	configuration.debugDevice = true;
	configuration.fullscreen = false;
	configuration.sampleCount = 1;
	configuration.sampleQuality = 0;
	configuration.vsync = true;

	return configuration;
}

} // anonymous namespace

App::App(const wm::MainArguments& mainArguments, const wm::Window::Configuration& mainWindowConfiguration) :
	wmApp_(mainArguments),
	mainWindow_(mainWindowConfiguration, essentials::make_observer(&wmApp_)),
	graphicsDevice_(mainWindow_.handle(), graphicsDeviceConfiguration()),
	imguiHost_(
		time::Timer(essentials::make_observer(&clock_)),
		graphicsDevice_,
		mainWindow_.clientWidth(),
		mainWindow_.clientHeight()
		)
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
	imguiHost_.update();

	graphicsDevice_.beginScene();

	{
		imguiHost_.render(graphicsDevice_, rendererCommandBuffer_);

		rendererCommandBuffer_.submit(graphicsDevice_.getImmediateCommandList());
	}

	graphicsDevice_.endScene();
}
