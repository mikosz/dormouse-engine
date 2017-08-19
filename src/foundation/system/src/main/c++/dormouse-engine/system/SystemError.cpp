#include "SystemError.hpp"

#include <sstream>

using namespace dormouse_engine::system;

std::string SystemError::constructMessage(const std::string& message, const std::error_code& theErrorCode) {
	std::ostringstream oss;
	if (!message.empty()) {
		oss << message;
	} else {
		oss << "System error.";
	}
	oss << '\n' << "Error code: " << theErrorCode.value()
		<< "\nCategory: " << theErrorCode.category().name()
		<< "\nError: " << theErrorCode.message();

	return oss.str();
}
