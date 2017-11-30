#ifndef _DORMOUSEENGINE_WM_APP_HPP_
#define _DORMOUSEENGINE_WM_APP_HPP_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/essentials/Singleton.hpp"
#include "dormouse-engine/essentials/policy/creation/New.hpp"
#include "Window.hpp"

namespace dormouse_engine::wm {

struct MainArguments {

	int argc;

	const char** argv;

	HINSTANCE hinstance;

	LPSTR commandLine;

	int showCommand;

	MainArguments() = default; // TODO: Singleton won't compile otherwise

	MainArguments(int argc, const char** argv, HINSTANCE hinstance, LPSTR commandLine, int showCommand) :
		argc(argc),
		argv(argv),
		hinstance(hinstance),
		commandLine(commandLine),
		showCommand(showCommand)
	{
	}

};

struct GlobalMainArguments :
	public MainArguments,
	public essentials::Singleton<
		MainArguments,
		essentials::policy::creation::New<MainArguments> // TODO: replace with None
		>
{
};

class MessagePump {
public:

	MessagePump(const MainArguments& mainArguments);

	void update();

	HINSTANCE instance() {
		return instance_;
	}

	LRESULT systemCallback(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
		return DefWindowProc(window, message, wparam, lparam);
	}

private:

	HINSTANCE instance_;

	LPSTR commandLine_;

	int showCommand_;

};

} // namespace dormouse_engine::wm

#define DE_APP_MAIN() \
	int CALLBACK WinMain(HINSTANCE instance__, HINSTANCE, LPSTR cmdLine__, int cmdShow__) { \
		{ \
			dormouse_engine::wm::GlobalMainArguments::setInstance( \
				std::make_unique<dormouse_engine::wm::MainArguments>( \
					__argc, const_cast<const char**>(__argv), instance__, cmdLine__, cmdShow__) \
				); \
		}

#endif /* _DORMOUSEENGINE_WM_APP_HPP_ */
