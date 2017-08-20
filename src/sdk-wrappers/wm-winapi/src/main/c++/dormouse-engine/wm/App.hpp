#ifndef _DORMOUSEENGINE_WM_APP_HPP_
#define _DORMOUSEENGINE_WM_APP_HPP_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/essentials/Singleton.hpp"
#include "dormouse-engine/essentials/policy/creation/New.hpp"
#include "Window.hpp"

namespace dormouse_engine::wm {

// TODO: extract to separate file
struct MainArguments {

	int argc;

	const char** argv;

	HINSTANCE hinstance;

	LPSTR commandLine;

	int showCommand;

	MainArguments() { // TODO: Singleton won't compile otherwise
	}

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
		essentials::creation::New<MainArguments> // TODO: replace with None
		>
{
};

class App {
public:

	App(const MainArguments& mainArguments);

	void update();

	bool closeRequested() const {
		return closeRequested_;
	}

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

	bool closeRequested_;

};

} // namespace dormouse_engine::wm

#define CT_BEGIN_MAIN() \
	int CALLBACK WinMain(HINSTANCE instance__, HINSTANCE, LPSTR cmdLine__, int cmdShow__) { \
		{ \
			coconut::milk::system::GlobalMainArguments::setInstance( \
				std::make_unique<coconut::milk::system::MainArguments>( \
					__argc, const_cast<const char**>(__argv), instance__, cmdLine__, cmdShow__) \
				); \
		}

#define CT_END_MAIN() }

#endif /* _DORMOUSEENGINE_WM_APP_HPP_ */
