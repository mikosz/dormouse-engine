#include "App.hpp"

#include "dormouse-engine/exceptions/RuntimeError.hpp"
#include "dormouse-engine/graphics/Adapter.hpp"
#include "dormouse-engine/essentials/debug.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::engine::app;

namespace /* anonymous */ {

graphics::Device createGraphicsDevice() {
	// #TODO: configuration?
	const auto graphicsAdapters = graphics::Adapter::create();

	if (graphicsAdapters.empty()) {
		throw exceptions::RuntimeError("No graphics adapters found");
	}

	return graphics::Device(graphicsAdapters[0], essentials::IS_DEBUG);
}

graphics::SwapChain createSwapChain(graphics::Device& graphicsDevice, wm::Window& window) {
	// #TODO: configuration?
	// #TODO: fullscreen won't work, because we don't use adapter::outputs() to poll for valid refresh rates
	auto configuration = graphics::SwapChain::Configuration();

	auto displayMode = graphics::DisplayMode();
	displayMode.width = window.clientWidth();
	displayMode.height = window.clientHeight();
	displayMode.pixelFormat = graphics::FORMAT_R8G8B8A8_UNORM;
	displayMode.refreshRateNumerator = 0;
	displayMode.refreshRateDenominator = 0;

	configuration.fullscreen = false;
	configuration.vsync = true;
	configuration.displayMode = std::move(displayMode);

	return graphics::SwapChain(graphicsDevice, window, configuration);
}

} // anonymous namespace

App::App(const wm::MainArguments& mainArguments, const wm::Window::Configuration& mainWindowConfiguration) :
	wmApp_(mainArguments),
	mainWindow_(mainWindowConfiguration, essentials::make_observer(&wmApp_)),
	graphicsDevice_(createGraphicsDevice()),
	swapChain_(createSwapChain(graphicsDevice_, mainWindow_)),
	imguiHost_(
		time::Timer(essentials::make_observer(&clock_)),
		graphicsDevice_,
		swapChain_.backBuffer(),
		mainWindow_.clientWidth(),
		mainWindow_.clientHeight()
		)
{
}

void App::run() {
	while (!wmApp_.closeRequested()) {
		frame();
	}
}

void App::frame() {
	clock_.tick();
	wmApp_.update();
	imguiHost_.update();

	onUpdateBroadcaster_.notify();

	swapChain_.clear();

	onRenderBroadcaster_.notify(rendererCommandBuffer_);

	imguiHost_.render(graphicsDevice_, rendererCommandBuffer_);

	rendererCommandBuffer_.submit(graphicsDevice_.getImmediateCommandList());

	swapChain_.present();
}
