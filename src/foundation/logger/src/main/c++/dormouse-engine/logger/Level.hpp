#ifndef DORMOUSEENGINE_LOGGER_LEVEL_HPP_
#define DORMOUSEENGINE_LOGGER_LEVEL_HPP_

#include <iosfwd>

#include <boost/algorithm/string/case_conv.hpp>

namespace dormouse_engine::logger {

enum class Level {
	TRACE,
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	CRITICAL,
};

std::istream& operator>>(std::istream& is, Level& level);

std::ostream& operator<<(std::ostream& os, Level level);

} // namespace dormouse_engine::logger

#endif /* DORMOUSEENGINE_LOGGER_LEVEL_HPP_ */
