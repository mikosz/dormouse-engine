#ifndef DORMOUSEENGINE_EXCEPTIONS_LOGICERROR_HPP_
#define DORMOUSEENGINE_EXCEPTIONS_LOGICERROR_HPP_

#include <stdexcept>

#include "GenericException.hpp"

namespace dormouse_engine::exceptions {

class LogicError : public GenericException<std::logic_error> {
public:

	EXCEPTION_CONSTRUCTORS(LogicError, GenericException<std::logic_error>)

};

} // namespace dormouse_engine::exceptions

#endif /* DORMOUSEENGINE_EXCEPTIONS_LOGICERROR_HPP_ */
