#ifndef DORMOUSEENGINE_EXCEPTIONS_LOGICERROR_HPP_
#define DORMOUSEENGINE_EXCEPTIONS_LOGICERROR_HPP_

#include <stdexcept>

#include "GenericException.hpp"

namespace dormouse_engine {
namespace exceptions {

class LogicError : public GenericException<std::logic_error> {
public:

	EXCEPTION_CONSTRUCTORS(LogicError, GenericException<std::logic_error>)

};

} // namespace exceptions
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_EXCEPTIONS_LOGICERROR_HPP_ */
