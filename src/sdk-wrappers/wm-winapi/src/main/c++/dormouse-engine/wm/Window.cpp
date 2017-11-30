#include "Window.hpp"

#include <cstring>
#include <stdexcept>
#include <sstream>

#include "dormouse-engine/system/windows/Error.hpp"
#include "MessagePump.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::wm;

Window::Window(const Configuration& configuration, essentials::observer_ptr<MessagePump> app) :
	configuration_(configuration),
	app_(app),
	handle_(nullptr)
{
	WNDCLASSEXA wndClassEx;
	std::memset(&wndClassEx, 0, sizeof(WNDCLASSEX));

	wndClassEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClassEx.lpfnWndProc = &messageHandler;
	wndClassEx.hInstance = app->instance();
	wndClassEx.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wndClassEx.hIconSm = wndClassEx.hIcon;
	wndClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClassEx.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wndClassEx.lpszClassName = configuration_.className.c_str();
	wndClassEx.cbSize = sizeof(WNDCLASSEX);

	if (RegisterClassExA(&wndClassEx) == 0) {
		std::ostringstream error;
		error << "Failed to register window class. Error code: " << GetLastError();

		throw std::runtime_error(error.str());
	}

	handle_ = CreateWindowExA(
		WS_EX_APPWINDOW,
		configuration_.className.c_str(),
		configuration_.title.c_str(),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		static_cast<int>(configuration_.width),
		static_cast<int>(configuration_.height),
		nullptr,
		nullptr,
		app_->instance(),
		this
		);

	if (handle_ == nullptr) {
		throw system::windows::Error(GetLastError(), "Failed to create a window.");
	}

	ShowWindow(handle_, SW_SHOW);
	SetForegroundWindow(handle_);
	SetFocus(handle_);

	ShowCursor(TRUE);
}

Window::~Window() {
	if (handle_ != nullptr) {
		DestroyWindow(handle_);
		handle_ = nullptr;
	}

	UnregisterClassA(configuration_.className.c_str(), app_->instance());
}

LRESULT CALLBACK Window::messageHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	switch (message) {
	case WM_NCCREATE:
		{
			LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lparam);
			Window* instance = reinterpret_cast<Window*>(createStruct->lpCreateParams);

			if (!instance) {
				throw std::logic_error("Window instance create parameter shall not be null!");
			}

			::SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));

			return TRUE;
		}
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return FALSE;
	default:
		{
			Window* instance = reinterpret_cast<Window*>(::GetWindowLongPtr(window, GWLP_USERDATA));

			if (instance) {
				return instance->app_->systemCallback(window, message, wparam, lparam);
			} else {
				return FALSE;
			}
		}
	}
}

size_t Window::clientWidth() const {
	RECT clientRect;
	if (!GetClientRect(handle_, &clientRect)) {
		throw std::runtime_error("Failed to retrieve client width");
	}

	return clientRect.right - clientRect.left;
}

size_t Window::clientHeight() const {
	RECT clientRect;
	if (!GetClientRect(handle_, &clientRect)) {
		throw std::runtime_error("Failed to retrieve client height");
	}

	return clientRect.bottom - clientRect.top;
}

bool Window::fullscreen() const {
	return configuration_.fullscreen;
}
