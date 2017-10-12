#ifndef _DORMOUSEENGINE_MATH_SI_HPP_
#define _DORMOUSEENGINE_MATH_SI_HPP_

#include <type_traits>
#include <ratio>

namespace dormouse_engine::math {

namespace detail {

template <int MExp, int KGExp, int SExp, class ToBaseRatioT>
struct Unit {

	using ToBaseRatio = ToBaseRatioT;
	static_assert(ToBaseRatio::num != 0 && ToBaseRatio::den != 0);

	static constexpr auto M_EXP = MExp;
	static constexpr auto KG_EXP = KGExp;
	static constexpr auto S_EXP = SExp;
	static constexpr auto TO_BASE = static_cast<float>(ToBaseRatio::num) / static_cast<float>(ToBaseRatio::den);

};

template <class UnitT>
class Value {
public:

	using Unit = UnitT;

	constexpr explicit Value(float value) noexcept :
		value_(value)
	{
	}

	constexpr const float value() const noexcept {
		return value_;
	}

private:

	float value_;

};

template <class UnitT>
constexpr inline bool operator==(Value<UnitT> lhs, Value<UnitT> rhs) noexcept {
	return lhs.value() == rhs.value();
}

template <class LhsUnitT, class RhsUnitT>
constexpr inline auto operator*(Value<LhsUnitT> lhs, Value<RhsUnitT> rhs) noexcept {
	return
		Value<Unit<
			LhsUnitT::M_EXP + RhsUnitT::M_EXP,
			LhsUnitT::KG_EXP + RhsUnitT::KG_EXP,
			LhsUnitT::S_EXP + RhsUnitT::S_EXP,
			std::ratio<
				LhsUnitT::ToBaseRatio::num * RhsUnitT::ToBaseRatio::num,
				LhsUnitT::ToBaseRatio::den * RhsUnitT::ToBaseRatio::den
				>
			>>(lhs.value() * rhs.value());
}

template <class LhsUnitT, class RhsUnitT>
constexpr inline auto operator/(Value<LhsUnitT> lhs, Value<RhsUnitT> rhs) noexcept {
	return
		Value<Unit<
			LhsUnitT::M_EXP - RhsUnitT::M_EXP,
			LhsUnitT::KG_EXP - RhsUnitT::KG_EXP,
			LhsUnitT::S_EXP - RhsUnitT::S_EXP,
			std::ratio<
				LhsUnitT::ToBaseRatio::num * RhsUnitT::ToBaseRatio::den,
				LhsUnitT::ToBaseRatio::den * RhsUnitT::ToBaseRatio::num
				>
			>>(lhs.value() / rhs.value());
}

} // namespace detail

// -- base SI units / values

template <class ToBaseRatio = std::ratio<1, 1>>
using Metre = detail::Unit<1, 0, 0, ToBaseRatio>;

template <class ToBaseRatio = std::ratio<1, 1>>
using Kilogram = detail::Unit<0, 1, 0, ToBaseRatio>;

template <class ToBaseRatio = std::ratio<1, 1>>
using Second = detail::Unit<0, 0, 1, ToBaseRatio>;

using Metres = detail::Value<Metre<>>;
using Kilograms = detail::Value<Kilogram<>>;
using Seconds = detail::Value<Second<>>;

constexpr inline Metres operator""_m(long double m) noexcept {
	return Metres(static_cast<float>(m));
}

constexpr inline Kilograms operator""_kg(long double kg) noexcept {
	return Kilograms(static_cast<float>(kg));
}

constexpr inline Seconds operator""_s(long double s) noexcept {
	return Seconds(static_cast<float>(s));
}

// -- common multipliers

using Kilometres = detail::Value<Metre<std::kilo>>;

constexpr inline Kilometres operator""_km(long double km) noexcept {
	return Kilometres(static_cast<float>(km));
}

using Hours = detail::Value<Second<std::ratio<60>>>;

constexpr inline Hours operator""_h(long double h) noexcept {
	return Hours(static_cast<float>(h));
}

// -- derived SI units / values

using Speed = decltype(1.0_m / 1.0_s);
using MetresPerSecond = Speed::Unit;

constexpr inline Speed operator""_mps(long double mps) noexcept {
	return Speed(static_cast<float>(mps));
}

using Acceleration = decltype(std::declval<Speed>() / 1.0_s);
using MetresPerSecondSq = Acceleration::Unit;

using Jerk = decltype(std::declval<Acceleration>() / 1.0_s);
using MetresPerSecondCu = Jerk::Unit;

using Force = decltype(1.0_kg * std::declval<Acceleration>());
using Newtons = Force::Unit;

constexpr inline Force operator""_N(long double n) noexcept {
	return Force(static_cast<float>(n));
}

} // namespace dormouse_engine::math

namespace dormouse_engine {

using math::Metre;
using math::Kilogram;
using math::Second;

using math::Metres;
using math::Kilograms;
using math::Seconds;

using math::Speed;
using math::MetresPerSecond;

using math::Acceleration;
using math::MetresPerSecondSq;

using math::Jerk;
using math::MetresPerSecondCu;

using math::Force;
using math::Newtons;

namespace math_literals {

using math::operator""_m;
using math::operator""_kg;
using math::operator""_s;

using math::operator""_mps;

using math::operator""_N;

} // namespace math_literals

} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_MATH_SI_HPP_ */
