#ifndef _DORMOUSEENGINE_ENGINE_APP_APP_HPP_
#define _DORMOUSEENGINE_ENGINE_APP_APP_HPP_

#include "dormouse-engine/wm/App.hpp"
#include "dormouse-engine/wm/Window.hpp"
#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/renderer/command/CommandBuffer.hpp"
#include "dormouse-engine/renderer/control/ResourceView.hpp"
#include "../time/WallClock.hpp"
#include "ImGuiHost.hpp"

struct ImDrawData;

namespace dormouse_engine::engine::app {

class App final {
public:

	App(const wm::MainArguments& mainArguments, const wm::Window::Configuration& mainWindowConfiguration);

	void run();

	void update();

	bool closeRequested() {
		return wmApp_.closeRequested();
	}

private:

	wm::App wmApp_;

	wm::Window mainWindow_;

	time::WallClock clock_;

	graphics::Device graphicsDevice_;

	renderer::command::CommandBuffer rendererCommandBuffer_;

	ImGuiHost imguiHost_;

};

} // namespace dormouse_engine::engine::app

#endif /* _DORMOUSEENGINE_ENGINE_APP_APP_HPP_ */
