#ifndef DORMOUSEENGINE_EXCEPTIONS_GENERICEXCEPTION_HPP_
#define DORMOUSEENGINE_EXCEPTIONS_GENERICEXCEPTION_HPP_

#include <string>
#include <sstream>
#include <exception>

#include "dormouse-engine/system/platform.hpp"

#if defined(DE_COMPILER_VISUAL_CXX)
#	include <intrin.h>
#	define DE_DEBUG_BREAK() __debugbreak()
#endif

#include "Backtrace.hpp"

namespace dormouse_engine::exceptions {

template <class ParentType>
class GenericException : public ParentType {
public:

	GenericException(const std::string& message) :
		ParentType(message),
		message_(message) {
	}

	GenericException(const std::string& message, const std::exception& cause) :
		ParentType(message),
		message_(message),
		cause_(cause.what()) {
	}

	virtual ~GenericException() = default;

	virtual const std::string& name() const noexcept = 0;

	const char* what() const noexcept override {
		return description().c_str();
	}

	const std::string& message() const noexcept {
		return message_;
	}

	const Backtrace& backtrace() const noexcept {
		return backtrace_;
	}

	const std::string& description() const noexcept {
		// description_ is built lazily, because we're calling name() which is virtual and
		// cannot be called in the constructor
		if (description_.empty()) {
			std::ostringstream oss;
			oss << name();
			if (!message_.empty()) {
				oss << "\nMessage: " << message_;
			}
			oss << '\n' << backtrace_;
			if (!cause_.empty()) {
				oss << "\nCaused by: " << cause_;
			}
			description_ = oss.str();
		}
		return description_;
	}

private:

	std::string message_, cause_;

	mutable std::string description_;

	Backtrace backtrace_;

};

#define EXCEPTION_CONSTRUCTORS(TYPE, PARENT) \
	TYPE() : \
		PARENT(#TYPE) \
	{ \
		DE_DEBUG_BREAK(); \
	} \
	\
	TYPE(const std::string& message) : \
		PARENT(message) \
	{ \
		DE_DEBUG_BREAK(); \
	} \
	\
	TYPE(const std::string& message, const std::exception& cause) : \
		PARENT(message, cause) \
	{ \
		DE_DEBUG_BREAK(); \
	} \
	\
	const std::string& name() const noexcept override { \
		static const std::string& NAME = #TYPE; \
		return NAME; \
	}

} // namespace dormouse_engine::exceptions

#endif /* DORMOUSEENGINE_EXCEPTIONS_GENERICEXCEPTION_HPP_ */
