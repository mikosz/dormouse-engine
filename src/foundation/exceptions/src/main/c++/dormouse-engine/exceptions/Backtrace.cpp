#include "Backtrace.hpp"

using namespace dormouse_engine::exceptions;

std::ostream& dormouse_engine::exceptions::operator<< (std::ostream& os, const Backtrace& backtrace) {
	return backtrace.print(os);
}
