#ifndef _DORMOUSEENGINE_SYSTEM_WINDOWS_ERROR_HPP_
#define _DORMOUSEENGINE_SYSTEM_WINDOWS_ERROR_HPP_

#include <string>
#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/exceptions/RuntimeError.hpp"

namespace dormouse_engine::system::windows {

class Error : public dormouse_engine::exceptions::RuntimeError {
public:

	Error(HRESULT hResult, const std::string& message);

	Error(DWORD errorCode, const std::string& message);

	const std::string& name() const noexcept override {
		static const std::string NAME = "WindowsError";
		return NAME;
	}

private:

	static std::string buildMessage(HRESULT hResult, const std::string& message);

	static std::string buildMessage(DWORD errorCode, const std::string& message);

};

inline void checkSystemCall(HRESULT result, const std::string& errorMessage) {
	if (FAILED(result)) {
		throw Error(result, errorMessage);
	}
}

} // namespace dormouse_engine::system::windows

#endif /* _DORMOUSEENGINE_SYSTEM_WINDOWS_ERROR_HPP_ */
