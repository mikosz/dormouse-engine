#ifndef DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_
#define DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_

#include <array>
#include <memory>
#include <type_traits>

#include "observer_ptr.hpp"

namespace dormouse_engine::essentials {

template <class... T>
constexpr auto makeArray(T&&... values) {
	return std::array<std::decay_t<std::common_type_t<T...>>, sizeof...(T)>{ std::forward<T>(values)... };
}

namespace detail {

template <class T>
struct IsAnyPointerHelper : std::false_type {
};

template <class T>
struct IsAnyPointerHelper<T*> : std::true_type {
};

template <class T>
struct IsAnyPointerHelper<std::unique_ptr<T>> : std::true_type {
};

template <class T>
struct IsAnyPointerHelper<std::shared_ptr<T>> : std::true_type {
};

template <class T>
struct IsAnyPointerHelper<observer_ptr<T>> : std::true_type {
};

} // namespace detail

template<class T>
struct IsAnyPointer : detail::IsAnyPointerHelper<std::remove_cv_t<T>> {
};

template<class T>
constexpr bool IsAnyPointer_v = IsAnyPointer<T>::value;

template <class T>
constexpr auto& dereference(T& value) {
	if constexpr (IsAnyPointer_v<T>) {
		return *value;
	} else {
		return value;
	}
}

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_ */
