#ifndef _DORMOUSEENGINE_WM_WINDOW_HPP_
#define _DORMOUSEENGINE_WM_WINDOW_HPP_

#include <memory>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/system/windows/types.hpp"

namespace dormouse_engine::wm {

class MessagePump;

class Window {
public:

	struct Configuration {

		size_t width;

		size_t height;

		bool fullscreen;

		std::string className;

		std::string title;

	};

	Window(const Configuration& configuration, essentials::observer_ptr<MessagePump> messagePump);

	~Window();

	size_t clientWidth() const;

	size_t clientHeight() const;

	bool fullscreen() const;

	system::windows::WindowHandle handle() {
		return handle_;
	}

private:

	Configuration configuration_;

	essentials::observer_ptr<MessagePump> messagePump_;

	system::windows::WindowHandle handle_;

	static LRESULT CALLBACK messageHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

};

} // namespace dormouse_engine::wm

#endif /* _DORMOUSEENGINE_WM_WINDOW_HPP_ */
