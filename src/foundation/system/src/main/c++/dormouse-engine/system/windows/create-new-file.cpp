#include "../platform.hpp"

#if defined(DE_PLATFORM_WINDOWS)

#include "create-new-file.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <boost/filesystem.hpp>

#include "../SystemError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::system;
using namespace dormouse_engine::system::windows;

bool dormouse_engine::system::windows::createNewFile(const boost::filesystem::path& path) {
	HANDLE h = CreateFile(path.wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, 0, 0);
	if (h != INVALID_HANDLE_VALUE) {
		CloseHandle(h);
		return true;
	} else {
		if (GetLastError() == ERROR_FILE_EXISTS) {
			return false;
		} else {
			throw SystemError("Failed to create file " + path.string(),
				std::error_code(GetLastError(), std::system_category()));
		}
	}
}

#endif /* DE_PLATFORM_WINDOWS */
