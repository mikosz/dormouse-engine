#ifndef DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_
#define DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_

#include <array>
#include <type_traits>

namespace dormouse_engine::essentials {

template <typename... T>
constexpr auto makeArray(T&&... values) {
	return std::array<std::decay_t<std::common_type_t<T...>>, sizeof...(T)>{ std::forward<T>(values)... };
}

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_ */
