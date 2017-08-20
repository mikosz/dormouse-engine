#include "Error.hpp"

#include <memory>
#include <sstream>

using namespace dormouse_engine::system::windows;

Error::Error(HRESULT hResult, const std::string& message) :
	dormouse_engine::exceptions::RuntimeError(buildMessage(hResult, message))
{
}

Error::Error(DWORD errorCode, const std::string& message) :
	dormouse_engine::exceptions::RuntimeError(buildMessage(errorCode, message))
{
}

std::string Error::buildMessage(DWORD errorCode, const std::string& message) {
	std::ostringstream oss;
	oss << message << " Error code: " << errorCode << " (0x" << std::hex << errorCode << ").";

	LPSTR messageBuffer;
	const auto size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&messageBuffer),
		0,
		nullptr
		);
	std::unique_ptr<LPSTR, decltype(&::LocalFree)> messageBufferDeleter(&messageBuffer, ::LocalFree);

	oss << " System message: " << messageBuffer;

	return oss.str();
}

std::string Error::buildMessage(HRESULT hResult, const std::string& message) {
	std::ostringstream oss;
	oss << message << " hResult: " << hResult << " (0x" << std::hex << hResult << ").";
	return oss.str();
}
