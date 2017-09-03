#ifndef DORMOUSEENGINE_ESSENTIALS_OBSERVERPTR_HPP_
#define DORMOUSEENGINE_ESSENTIALS_OBSERVERPTR_HPP_

#include <cassert>
#include <algorithm>
#include <functional>
#include <type_traits>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

namespace dormouse_engine::essentials {

template <class T>
class observer_ptr {
public:

	using element_type = T;
	using pointer = T*;
	using reference = T&;

	constexpr observer_ptr() noexcept :
		ptr_(nullptr)
	{
	}

	constexpr observer_ptr(std::nullptr_t) noexcept :
		ptr_(nullptr)
	{
	}

	constexpr explicit observer_ptr(pointer p) noexcept :
		ptr_(p)
	{
	}

	template <class U>
	constexpr observer_ptr(observer_ptr<U> other) noexcept
		: ptr_(other.get())
	{
	}

	constexpr pointer get() const noexcept {
		return ptr_;
	}

	constexpr reference operator*() const noexcept {
		assert(ptr_ != nullptr);
		return *ptr_;
	}

	constexpr pointer operator->() const noexcept {
		return ptr_;
	}

	constexpr explicit operator bool() const noexcept {
		return ptr_ != nullptr;
	}

	constexpr operator pointer() const noexcept {
		return ptr_;
	}

	constexpr pointer release() noexcept {
		pointer p(ptr_);
		reset();
		return p;
	}

	constexpr void reset(pointer p = nullptr) noexcept {
		ptr_ = p;
	}

	constexpr void swap(observer_ptr& other) noexcept {
		using std::swap;
		swap(ptr_, other.ptr_);
	}

private:

	pointer ptr_;

};

// specialized algorithms:

template <class T>
void swap(observer_ptr<T> & lhs, observer_ptr<T> & rhs) noexcept {
	lhs.swap(rhs);
}

template <class T>
observer_ptr<T> make_observer(T* p) noexcept {
	return observer_ptr<T>(p);
}

template <class T, class U>
bool operator==(observer_ptr<T> lhs, observer_ptr<U> rhs) {
	return lhs.get() == rhs.get();
}

template <class T, class U>
bool operator!=(observer_ptr<T> lhs, observer_ptr<U> rhs) {
	return !(lhs == rhs);
}

template <class T>
bool operator==(observer_ptr<T> p, std::nullptr_t) noexcept {
	return !p;
}

template <class T>
bool operator==(std::nullptr_t, observer_ptr<T> p) noexcept {
	return !p;
}

template <class T>
bool operator!=(observer_ptr<T> p, std::nullptr_t) noexcept {
	return static_cast<bool>(p);
}

template <class T>
bool operator!=(std::nullptr_t, observer_ptr<T> p) noexcept {
	return static_cast<bool>(p);
}

template <class T, class U>
bool operator<(observer_ptr<T> lhs, observer_ptr<U> rhs) {
	return std::less<std::common_type_t<T*, U*>>()(lhs.get(), rhs.get());
}

template <class T, class U>
bool operator>(observer_ptr<T> lhs, observer_ptr<U> rhs) {
	return rhs < lhs;
}

template <class T, class U>
bool operator<=(observer_ptr<T> lhs, observer_ptr<U> rhs) {
	return !(rhs < lhs);
}

template <class T, class U>
bool operator>=(observer_ptr<T> lhs, observer_ptr<U> rhs) {
	return !(lhs < rhs);
}

} // namespace dormouse_engine::essentials

namespace std {

template <class T>
struct hash<::dormouse_engine::essentials::observer_ptr<T>> {

	constexpr size_t operator()(::dormouse_engine::essentials::observer_ptr<T> p)
		noexcept(noexcept(hash<T*>()(p.get())))
	{
		return hash<T*>()(p.get());
	}

};

} // namespace std

#endif /* DORMOUSEENGINE_ESSENTIALS_OBSERVERPTR_HPP_ */
