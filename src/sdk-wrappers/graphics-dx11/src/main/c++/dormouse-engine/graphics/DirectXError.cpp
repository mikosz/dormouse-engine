#include "DirectXError.hpp"

#include <sstream>

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

DirectXError::DirectXError(HRESULT errorCode, const std::string& message) :
	dormouse_engine::exceptions::RuntimeError(buildMessage(errorCode, message)),
	errorCode_(errorCode)
{
}

std::string DirectXError::buildMessage(HRESULT errorCode, const std::string& message) {
	std::ostringstream oss;
	oss << "DirectX error occurred. Error code: " << errorCode << ". Message: " << message;
	return oss.str();
}
