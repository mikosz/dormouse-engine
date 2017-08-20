#ifndef _DORMOUSEENGINE_GRAPHICS_DIRECTXERROR_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DIRECTXERROR_HPP_

#include <string>
#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/exceptions/RuntimeError.hpp"

namespace dormouse_engine {
namespace graphics {

class DirectXError : public dormouse_engine::exceptions::RuntimeError {
public:

	DirectXError(HRESULT errorCode, const std::string& message);

	const std::string& name() const noexcept override {
		static const std::string NAME = "DirectXError";
		return NAME;
	}

	HRESULT errorCode() const {
		return errorCode_;
	}

private:

	HRESULT errorCode_;

	static std::string buildMessage(HRESULT errorCode, const std::string& message);

};

inline void checkDirectXCall(HRESULT result, const std::string& errorMessage) {
	if (FAILED(result)) {
		throw DirectXError(result, errorMessage);
	}
}

} // namespace graphics
} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_GRAPHICS_DIRECTXERROR_HPP_ */
