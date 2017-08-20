#ifndef _DORMOUSEENGINE_ESSENTIALS_BITS_HPP_
#define _DORMOUSEENGINE_ESSENTIALS_BITS_HPP_

#include <cassert>
#include <cstdint>
#include <type_traits>

namespace dormouse_engine::essentials {

namespace detail {

union {
	std::uint16_t value;
	unsigned char bytes[2];
} constexpr static one{ 1 };

} // namespace detail

constexpr bool isBigEndian() noexcept {
	return detail::one.bytes[1] == 1;
}

template <class T, std::enable_if_t<std::is_arithmetic<T>::value>* = nullptr>
T changeEndianness(T value) noexcept {
	T result;
	auto* writer = reinterpret_cast<std::uint8_t*>(&result);
	const auto* firstByte = reinterpret_cast<const std::uint8_t*>(&value);
	for (const auto* byte = firstByte + sizeof(value); byte > firstByte; --byte) {
		*writer++ = *(byte - 1);
	}

	return result;
}

template <class T, class U>
T roundUpToMultipleOf(T value, U multiple) {
	assert(multiple != 0);

	T remainder = value % multiple;

	if (remainder == 0) {
		return value;
	} else {
		return value + multiple - remainder;
	}
}

template <class T>
size_t numberOfBitsOn(T value) {
	size_t bitsOn = 0;

	while (value != 0) {
		auto minusOne = value - 1;
		value = value & minusOne;
		++bitsOn;
	}

	return bitsOn;
}

} // namespace dormouse_engine::essentials

#endif /* _DORMOUSEENGINE_ESSENTIALS_BITS_HPP_ */
