#include "Backtrace.hpp"

#include <ostream>

using namespace dormouse_engine::exceptions::no_backtrace;

std::ostream& Backtrace::print(std::ostream& os) const {
	return os << "-- backtrace unavailable --";
}
