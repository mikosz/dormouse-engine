#ifndef DORMOUSEENGINE_EXCEPTIONS_NO_BACKTRACE_BACKTRACE_HPP_
#define DORMOUSEENGINE_EXCEPTIONS_NO_BACKTRACE_BACKTRACE_HPP_

#include <string>
#include <vector>
#include <iosfwd>

namespace dormouse_engine::exceptions::no_backtrace {

class Backtrace {
public:

	std::ostream& print(std::ostream& os) const;

};

} // namespace dormouse_engine::exceptions::no_backtrace

#endif /* DORMOUSEENGINE_EXCEPTIONS_NO_BACKTRACE_BACKTRACE_HPP_ */
