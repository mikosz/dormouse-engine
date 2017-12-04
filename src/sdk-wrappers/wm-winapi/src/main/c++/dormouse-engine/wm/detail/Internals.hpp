#ifndef _DORMOUSEENGINE_WM_DETAIL_INTERNALS_HPP_
#define _DORMOUSEENGINE_WM_DETAIL_INTERNALS_HPP_

#include "../MessagePump.hpp"

namespace dormouse_engine::wm::detail {

struct Internals {
	
	static HINSTANCE winHInstance(const MessagePump& messagePump) {
		return messagePump.instance_;
	}

	static LRESULT systemCallback(const MessagePump& messagePump, HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
		return messagePump.systemCallback_(window, message, wparam, lparam);
	}

};

} // namespace dormouse_engine::wm::detail

#endif /* _DORMOUSEENGINE_WM_DETAIL_INTERNALS_HPP_ */
