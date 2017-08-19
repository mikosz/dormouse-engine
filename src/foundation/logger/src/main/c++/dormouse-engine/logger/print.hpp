#ifndef DORMOUSEENGINE_LOGGER_PRINT_HPP_
#define DORMOUSEENGINE_LOGGER_PRINT_HPP_

#include <iosfwd>
#include <type_traits>

#include "dormouse-engine/essentials/IsIterable.hpp"

namespace dormouse_engine {
namespace logger {

template<class T>
inline typename std::enable_if_t<!essentials::IsIterable<T>::value> print(std::ostream& os, const T& v) {
	os << v;
}

template <class T>
inline typename std::enable_if_t<essentials::IsIterable<T>::value> print(std::ostream& os, const T& iterable) {
	using std::begin;
	using std::end;

	auto it = begin(iterable), endIt = end(iterable);

	if (it != endIt) {
		os << *it;
		++it;
	}

	while (it != endIt) {
		os << ", " << *it;
		++it;
	}
}

inline void print(std::ostream& os, const std::string& s) {
	os << s;
}

inline void print(std::ostream& os, const char* s) {
	os << s;
}

}  // namespace logger
}  // namespace dormouse_engine

#endif /* DORMOUSEENGINE_LOGGER_PRINT_HPP_ */
