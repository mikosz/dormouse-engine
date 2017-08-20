#ifndef DORMOUSEENGINE_UTILS_LOCALTIMETHREADSAFE_HPP_
#define DORMOUSEENGINE_UTILS_LOCALTIMETHREADSAFE_HPP_

#include <ctime>
#include <mutex>

#include "dormouse-engine/exceptions/LogicError.hpp"

#include "dormouse-engine/system/platform.hpp"

#include "dormouse-engine/essentials/pointee.hpp"

namespace dormouse_engine::essentials {

void localtime(std::tm* tmPtr, std::time_t t) {
	std::tm& tm = pointee(tmPtr);

#if defined(DE_COMPILER_VISUAL_CXX)
	if (localtime_s(&tm, &t) != 0) {
		throw exceptions::LogicError("Failed to retrieve localtime");
	}
#else
	static std::mutex localtimeMutex;
	std::unique_lock<std::mutex> localtimeLock(localtimeMutex);

	auto result = std::localtime(&t);
	if (!result) {
		throw exceptions::LogicError("Failed to retrieve localtime");
	}

	tm = *result;
#endif /* DE_COMPILER_VISUAL_CXX */
}

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_UTILS_LOCALTIMETHREADSAFE_HPP_ */
