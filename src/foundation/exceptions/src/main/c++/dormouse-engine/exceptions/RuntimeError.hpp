#ifndef DORMOUSEENGINE_EXCEPTIONS_RUNTIMEERROR_HPP_
#define DORMOUSEENGINE_EXCEPTIONS_RUNTIMEERROR_HPP_

#include <stdexcept>

#include "GenericException.hpp"

namespace dormouse_engine::exceptions {

class RuntimeError : public GenericException<std::runtime_error> {
public:

	EXCEPTION_CONSTRUCTORS(RuntimeError, GenericException<std::runtime_error>)

};

} // namespace dormouse_engine::exceptions

#endif /* DORMOUSEENGINE_EXCEPTIONS_RUNTIMEERROR_HPP_ */
