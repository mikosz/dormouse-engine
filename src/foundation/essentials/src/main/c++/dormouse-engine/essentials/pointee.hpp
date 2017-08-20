#ifndef DORMOUSEENGINE_ESSENTIALS_POINTEE_HPP_
#define DORMOUSEENGINE_ESSENTIALS_POINTEE_HPP_

#include "dormouse-engine/exceptions/RuntimeError.hpp"

namespace dormouse_engine::essentials {

class NullPointerException : public exceptions::RuntimeError {
public:

	NullPointerException() :
		exceptions::RuntimeError("Attempted to dereference a null pointer.") {
	}

	~NullPointerException() throw () {
	}

};

template <class T>
T& pointee(T* ptr) {
	if (!ptr) {
		throw NullPointerException();
	}
	return *ptr;
}

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_ESSENTIALS_POINTEE_HPP_ */
