#ifndef DORMOUSEENGINE_ENUMS_MASK_HPP_
#define DORMOUSEENGINE_ENUMS_MASK_HPP_

#include <type_traits>
#include <iosfwd>
#include <bitset>

#include "flag.hpp"

namespace dormouse_engine::enums {

template <class FlagType>
class Mask {
public:

	using Flag = FlagType;

	using IntegralType = std::underlying_type_t<Flag>;

	constexpr Mask() noexcept :
		mask_(IntegralType(0))
	{
	}

	constexpr Mask(Flag flag) noexcept :
		mask_(static_cast<IntegralType>(flag))
	{
		static_assert(IsFlagV<Flag>, "Mask template type is not a flag type");
	}

	constexpr explicit operator bool() const noexcept {
		return mask_ != IntegralType(0);
	}

	friend constexpr bool operator ==(Mask lhs, Mask rhs) noexcept {
		return lhs.mask_ == rhs.mask_;
	}

	friend constexpr bool operator !=(Mask lhs, Mask rhs) noexcept {
		return lhs.mask_ != rhs.mask_;
	}

	friend constexpr Mask operator|(Mask lhs, Mask rhs) noexcept {
		return lhs.mask_ | rhs.mask_;
	}

	friend constexpr Mask operator&(Mask lhs, Mask rhs) noexcept {
		return lhs.mask_ & rhs.mask_;
	}

	friend constexpr Mask operator^(Mask lhs, Mask rhs) noexcept {
		return lhs.mask_ ^ rhs.mask_;
	}

	friend constexpr Mask operator~(Mask m) noexcept {
		return ~m.mask_;
	}

	friend std::ostream& operator<<(std::ostream& os, const Mask& mask) {
		return os << std::bitset<8 * sizeof(IntegralType)>(mask.mask_);
	}

	constexpr IntegralType integralValue() const noexcept {
		return mask_;
	}

private:

	IntegralType mask_;

	constexpr Mask(IntegralType i) noexcept :
		mask_(i)
	{
	}

};

template <class FlagType, class = std::enable_if_t<IsFlagV<FlagType>>>
constexpr Mask<FlagType> operator|(FlagType lhs, FlagType rhs) noexcept {
	return Mask<FlagType>() | lhs | rhs;
}

template <class FlagType, class = std::enable_if_t<IsFlagV<FlagType>>>
constexpr Mask<FlagType> operator&(FlagType lhs, FlagType rhs) noexcept {
	return Mask<FlagType>(lhs) & rhs;
}

template <class FlagType, class = std::enable_if_t<IsFlagV<FlagType>>>
constexpr Mask<FlagType> operator^(FlagType lhs, FlagType rhs) noexcept {
	return Mask<FlagType>() ^ lhs ^ rhs;
}

} // namespace dormouse_engine::enums

namespace dormouse_engine {

using enums::Mask;

} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_ENUMS_MASK_HPP_ */
