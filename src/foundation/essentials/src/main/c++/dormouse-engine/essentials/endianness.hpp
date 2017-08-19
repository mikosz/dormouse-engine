#ifndef _DORMOUSEENGINE_BITS_ENDIANNESS_HPP_
#define _DORMOUSEENGINE_BITS_ENDIANNESS_HPP_

#include <cstdint>
#include <type_traits>

namespace dormouse_engine {
namespace essentials {

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

} // namespace essentials
} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_BITS_ENDIANNESS_HPP_ */
