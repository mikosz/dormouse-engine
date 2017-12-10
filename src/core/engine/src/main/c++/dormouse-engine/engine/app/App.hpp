#ifndef _DORMOUSEENGINE_ENGINE_APP_APP_HPP_
#define _DORMOUSEENGINE_ENGINE_APP_APP_HPP_

#include "dormouse-engine/essentials/event.hpp"
#include "dormouse-engine/wm/MessagePump.hpp"
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

	struct OnUpdateEvent {
	};

	struct OnRenderEvent {
	};

	using EventBroadcaster = essentials::EventBroadcaster<
		OnUpdateEvent,
		OnRenderEvent
		>;

	App(const wm::MainArguments& mainArguments, const wm::Window::Configuration& mainWindowConfiguration);

	void run();

	void frame();

	bool closeRequested() const noexcept {
		return closeRequested_;
	}

	wm::Window& mainWindow() noexcept {
		return mainWindow_;
	}

	graphics::Device& graphicsDevice() noexcept {
		return graphicsDevice_;
	}

	EventBroadcaster& eventBroadcaster() noexcept {
		return eventBroadcaster_;
	}

private:

	wm::MessagePump wmMessagePump_;

	bool closeRequested_ = false;

	wm::Window mainWindow_;

	time::WallClock clock_;

	graphics::Device graphicsDevice_;

	graphics::SwapChain swapChain_;

	renderer::command::CommandBuffer rendererCommandBuffer_;

	ImGuiHost imguiHost_;

	EventBroadcaster eventBroadcaster_;

};

} // namespace dormouse_engine::engine::app

namespace dormouse_engine::engine {

using app::App;

} // namespace dormouse_engine::engine

#endif /* _DORMOUSEENGINE_ENGINE_APP_APP_HPP_ */
