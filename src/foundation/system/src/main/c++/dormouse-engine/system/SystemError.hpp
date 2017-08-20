#ifndef DORMOUSEENGINE_SYSTEM_SYSTEMERROR_HPP_
#define DORMOUSEENGINE_SYSTEM_SYSTEMERROR_HPP_

#include <system_error>

#include "dormouse-engine/exceptions/RuntimeError.hpp"

namespace dormouse_engine::system {

class SystemError : public exceptions::RuntimeError {
public:

	SystemError(const std::error_code& theErrorCode) :
		exceptions::RuntimeError(constructMessage(std::string(), theErrorCode)),
		errorCode_(theErrorCode)
	{
	}

	SystemError(const std::string& theMessage, const std::error_code& theErrorCode) :
		exceptions::RuntimeError(constructMessage(theMessage, theErrorCode))
	{
	}

	const std::error_code& errorCode() {
		return errorCode_;
	}

private:

	std::error_code errorCode_;

	static std::string constructMessage(const std::string& message, const std::error_code& theErrorCode);

};

} // namespace dormouse_engine::system

#endif /* DORMOUSEENGINE_SYSTEM_SYSTEMERROR_HPP_ */
