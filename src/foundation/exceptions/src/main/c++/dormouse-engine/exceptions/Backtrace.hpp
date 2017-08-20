#ifndef DORMOUSEENGINE_EXCEPTIONS_BACKTRACE_HPP_
#define DORMOUSEENGINE_EXCEPTIONS_BACKTRACE_HPP_

#include <iosfwd>

#include "dormouse-engine/system/platform.hpp"

#if defined(DE_COMPILER_GCC) && !defined(DE_NO_BACKTRACE)
#	include "gcc/Backtrace.hpp"
namespace dormouse_engine { namespace exceptions { using gcc::Backtrace; } }
#else
#	include "no-backtrace/Backtrace.hpp"
namespace dormouse_engine { namespace exceptions { using no_backtrace::Backtrace; } }
#endif

namespace dormouse_engine::exceptions {

std::ostream& operator<< (std::ostream& os, const Backtrace& backtrace);

} // namespace dormouse_engine::exceptions

#endif /* DORMOUSEENGINE_EXCEPTIONS_BACKTRACE_HPP_ */
