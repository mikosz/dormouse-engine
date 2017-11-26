#ifndef _DORMOUSEENGINE_ENGINE_APP_APP_HPP_
#define _DORMOUSEENGINE_ENGINE_APP_APP_HPP_

#include "dormouse-engine/essentials/event.hpp"
#include "dormouse-engine/wm/App.hpp"
#include "dormouse-engine/wm/Window.hpp"
#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/SwapChain.hpp"
#include "dormouse-engine/renderer/command/CommandBuffer.hpp"
#include "dormouse-engine/renderer/control/ResourceView.hpp"
#include "../time/WallClock.hpp"
#include "ImGuiHost.hpp"

struct ImDrawData;

namespace dormouse_engine::engine::app {

class App final {
public:

	using OnUpdateListener = EventListener<>;

	using OnUpdateRegistrar = EventBroadcaster<>::ListenerRegistrar;

	using OnRenderListener = EventListener<renderer::command::CommandBuffer&>;

	using OnRenderRegistrar = EventBroadcaster<renderer::command::CommandBuffer&>::ListenerRegistrar;

	App(const wm::MainArguments& mainArguments, const wm::Window::Configuration& mainWindowConfiguration);

	void run();

	void frame();

	bool closeRequested() {
		return wmApp_.closeRequested();
	}

	OnUpdateRegistrar subscribeToOnUpdate(OnUpdateListener listener) {
		return onUpdateBroadcaster_.subscribe(std::move(listener));
	}

	OnRenderRegistrar subscribeToOnRender(OnRenderListener listener) {
		return onRenderBroadcaster_.subscribe(std::move(listener));
	}

	wm::Window& mainWindow() noexcept {
		return mainWindow_;
	}

	graphics::Device& graphicsDevice() noexcept {
		return graphicsDevice_;
	}

private:

	using OnUpdateBroadcaster = EventBroadcaster<>;

	using OnRenderBroadcaster = EventBroadcaster<renderer::command::CommandBuffer&>;

	wm::App wmApp_;

	wm::Window mainWindow_;

	time::WallClock clock_;

	graphics::Device graphicsDevice_;

	graphics::SwapChain swapChain_;

	renderer::command::CommandBuffer rendererCommandBuffer_;

	ImGuiHost imguiHost_;

	OnUpdateBroadcaster onUpdateBroadcaster_;

	OnRenderBroadcaster onRenderBroadcaster_;

};

} // namespace dormouse_engine::engine::app

namespace dormouse_engine::engine {

using app::App;

} // namespace dormouse_engine::engine

#endif /* _DORMOUSEENGINE_ENGINE_APP_APP_HPP_ */
