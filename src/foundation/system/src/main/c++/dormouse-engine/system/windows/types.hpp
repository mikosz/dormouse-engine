#ifndef _DORMOUSEENGINE_SYSTEM_WINDOWS_TYPES_HPP_
#define _DORMOUSEENGINE_SYSTEM_WINDOWS_TYPES_HPP_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "cleanup-macros.hpp"

namespace dormouse_engine::system::windows {

using WindowHandle = HWND;

} // namespace dormouse_engine::system::windows

#endif /* _DORMOUSEENGINE_SYSTEM_WINDOWS_TYPES_HPP_ */
