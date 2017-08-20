#ifndef DORMOUSEENGINE_IS_ITERABLE_HPP_
#define DORMOUSEENGINE_IS_ITERABLE_HPP_

#include <iterator>

namespace dormouse_engine::essentials {

namespace detail {

using std::begin;
using std::end;

template <typename T>
auto IsIterable(int) -> decltype(
	begin(std::declval<T&>()) != end(std::declval<T&>()), // begin/end and operator !=
	++std::declval<decltype(begin(std::declval<T&>()))&>(), // operator ++
	*begin(std::declval<T&>()), // operator*
	std::true_type()
);

template <typename T>
std::false_type IsIterable(...);

} // namespace detail

template <typename T>
struct IsIterable : decltype(detail::IsIterable<T>(0)) {
};

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_IS_ITERABLE_HPP_ */
