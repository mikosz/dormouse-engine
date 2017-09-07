#ifndef DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_
#define DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_

#include <array>
#include <type_traits>

namespace dormouse_engine::essentials {

template <class... T>
constexpr auto makeArray(T&&... values) {
	return std::array<std::decay_t<std::common_type_t<T...>>, sizeof...(T)>{ std::forward<T>(values)... };
}

template <class T>
constexpr auto& dereference(T& value) {
	if constexpr (std::is_pointer_v<T>) {
		return *value;
	} else {
		return value;
	}
}

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_ */
