#ifndef _DORMOUSEENGINE_MATH_SI_HPP_
#define _DORMOUSEENGINE_MATH_SI_HPP_

#include <type_traits>

namespace dormouse_engine::math {

namespace detail {

template <int MExp, int KGExp, int SExp>
struct Unit {

	static constexpr auto M_EXP = MExp;
	static constexpr auto KG_EXP = KGExp;
	static constexpr auto S_EXP = SExp;

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
			LhsUnitT::S_EXP + RhsUnitT::S_EXP
			>>(lhs.value() * rhs.value());
}

template <class LhsUnitT, class RhsUnitT>
constexpr inline auto operator/(Value<LhsUnitT> lhs, Value<RhsUnitT> rhs) noexcept {
	return
		Value<Unit<
			LhsUnitT::M_EXP - RhsUnitT::M_EXP,
			LhsUnitT::KG_EXP - RhsUnitT::KG_EXP,
			LhsUnitT::S_EXP - RhsUnitT::S_EXP
			>>(lhs.value() / rhs.value());
}

} // namespace detail

using Metres = detail::Unit<1, 0, 0>;
using Kilograms = detail::Unit<0, 1, 0>;
using Seconds = detail::Unit<0, 0, 1>;

using Length = detail::Value<Metres>;
using Mass = detail::Value<Kilograms>;
using Time = detail::Value<Seconds>;

constexpr inline Length operator""_m(long double m) noexcept {
	return Length(static_cast<float>(m));
}

constexpr inline Mass operator""_kg(long double kg) noexcept {
	return Mass(static_cast<float>(kg));
}

constexpr inline Time operator""_s(long double s) noexcept {
	return Time(static_cast<float>(s));
}

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

using math::Metres;
using math::Kilograms;
using math::Seconds;

using math::Length;
using math::Mass;
using math::Time;

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
