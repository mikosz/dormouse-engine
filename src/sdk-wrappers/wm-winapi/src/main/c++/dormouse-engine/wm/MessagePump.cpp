#include "MessagePump.hpp"

using namespace dormouse_engine::wm;

MessagePump::MessagePump(const MainArguments& mainArguments) :
	instance_(mainArguments.hinstance),
	commandLine_(mainArguments.commandLine),
	showCommand_(mainArguments.showCommand)
{
}

void MessagePump::update() {
	auto message = MSG();
	while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT) {
			closeRequested_ = true;
		}
	}
}
