#ifndef _DORMOUSEENGINE_MATH_SI_HPP_
#define _DORMOUSEENGINE_MATH_SI_HPP_

#include <string>
#include <sstream>
#include <type_traits>
#include <ratio>

namespace dormouse_engine::math {

namespace detail {

template <int MExp, int KGExp, int SExp, class ToBaseRatioT>
struct Unit final {

	using ToBaseRatio = ToBaseRatioT;
	static_assert(ToBaseRatio::num != 0 && ToBaseRatio::den != 0);

	static constexpr auto M_EXP = MExp;
	static constexpr auto KG_EXP = KGExp;
	static constexpr auto S_EXP = SExp;
	static constexpr auto TO_BASE = static_cast<float>(ToBaseRatio::num) / static_cast<float>(ToBaseRatio::den);

	template <class OtherUnitT>
	static constexpr auto CONVERTIBLE_TO_v = (
		M_EXP == OtherUnitT::M_EXP &&
		KG_EXP == OtherUnitT::KG_EXP &&
		S_EXP == OtherUnitT::S_EXP
		);

	template <class OtherUnitT>
	using MultipliedBy_t = Unit<
		M_EXP + OtherUnitT::M_EXP,
		KG_EXP + OtherUnitT::KG_EXP,
		S_EXP + OtherUnitT::S_EXP,
		std::ratio<
			ToBaseRatio::num * OtherUnitT::ToBaseRatio::num,
			ToBaseRatio::den * OtherUnitT::ToBaseRatio::den
			>
		>;

	template <class OtherUnitT>
	using DividedBy_t = Unit<
		M_EXP - OtherUnitT::M_EXP,
		KG_EXP - OtherUnitT::KG_EXP,
		S_EXP - OtherUnitT::S_EXP,
		std::ratio<
			ToBaseRatio::num * OtherUnitT::ToBaseRatio::den,
			ToBaseRatio::den * OtherUnitT::ToBaseRatio::num
			>
		>;
};

template <int MExp, int KGExp, int SExp, class ToBaseRatioT>
inline std::string to_string(Unit<MExp, KGExp, SExp, ToBaseRatioT>) {
	using std::to_string;

	auto numerator = std::string();
	auto denominator = std::string();

	const auto appendStarIfNotEmpty = [](std::string& s) {
			if (!s.empty()) {
				s += '*';
			}
		};

	const auto appendExponent = [](std::string& s, int exponent) {
			const auto absExponent = std::abs(exponent);
			if (absExponent > 1) {
				s += "^" + to_string(absExponent);
			}
		};

	if constexpr (KGExp > 0) {
		numerator += "kg"s;
		appendExponent(numerator, KGExp);
	} else if constexpr (KGExp < 0) {
		denominator += "kg"s;
		appendExponent(denominator, KGExp);
	}

	if constexpr (MExp > 0) {
		appendStarIfNotEmpty(numerator);
		numerator += "m"s;
		appendExponent(numerator, MExp);
	} else if constexpr (MExp < 0) {
		appendStarIfNotEmpty(denominator);
		denominator += "m"s;
		appendExponent(denominator, MExp);
	}

	if constexpr (SExp > 0) {
		appendStarIfNotEmpty(numerator);
		numerator += "s"s;
		appendExponent(numerator, SExp);
	} else if constexpr (SExp < 0) {
		appendStarIfNotEmpty(denominator);
		denominator += "s"s;
		appendExponent(denominator, SExp);
	}

	auto result = std::string();

	if (numerator.empty()) {
		numerator = "1"s;
	}

	if (!denominator.empty()) {
		denominator = "/" + denominator;
	}

	auto ratio = std::string();

	if constexpr (ToBaseRatioT::den != 1) {
		ratio = to_string(ToBaseRatioT::num) + "/" + to_string(ToBaseRatioT::den) + "*";
	} else if constexpr (ToBaseRatioT::num != 1) {
		ratio = to_string(ToBaseRatioT::num) + "*";
	}

	return "[" + ratio + numerator + denominator + "]";
}

inline std::string to_string(Unit<1, 1, -2, std::ratio<1>>) noexcept {
	return "[N]";
}

inline std::string to_string(Unit<1, 0, -1, std::ratio<1000, 3600>>) noexcept {
	return "[km/h]";
}

template <class ToBaseRatioT>
using LengthUnit = detail::Unit<1, 0, 0, ToBaseRatioT>;

template <class ToBaseRatioT>
using MassUnit = detail::Unit<0, 1, 0, ToBaseRatioT>;

template <class ToBaseRatioT>
using TimeUnit = detail::Unit<0, 0, 1, ToBaseRatioT>;

template <class SourceUnitT, class TargetUnitT>
constexpr float convert(float value) noexcept {
	static_assert(SourceUnitT::template CONVERTIBLE_TO_v<TargetUnitT>);
	return value * SourceUnitT::TO_BASE / TargetUnitT::TO_BASE;
}

template <class UnitT>
class Value final {
public:

	using Unit = UnitT;

	Value() = default;

	template <class CompatibleUnitT>
	constexpr Value(Value<CompatibleUnitT> other) noexcept :
		value_(other.template value<UnitT>())
	{
	}

	template <class SourceUnitT>
	constexpr explicit Value(float value, SourceUnitT) noexcept :
		value_(convert<SourceUnitT, UnitT>(value))
	{
	}

	template <class TargetUnitT>
	constexpr const float value() const noexcept {
		return convert<UnitT, TargetUnitT>(value_);
	}

	constexpr Value operator-() const noexcept {
		return Value(-value_, Unit());
	}

	constexpr Value& operator+=(UnitT other) noexcept {
		value_ += other.value_;
		return *this;
	}

	constexpr Value& operator-=(UnitT other) noexcept {
		value_ -= other.value_;
		return *this;
	}

	constexpr Value& operator*=(float dimensionless) noexcept {
		value_ *= dimensionless;
		return *this;
	}

	constexpr Value& operator/=(float dimensionless) noexcept {
		value_ /= dimensionless;
		return *this;
	}

private:

	float value_;

};

template <class UnitT>
constexpr auto makeValue(float value) noexcept {
	return Value<UnitT>(value, UnitT());
}

template <class LhsUnitT, class RhsUnitT>
constexpr inline bool operator==(Value<LhsUnitT> lhs, Value<RhsUnitT> rhs) noexcept {
	return lhs.template value<LhsUnitT>() == rhs.template value<LhsUnitT>();
}

template <class LhsUnitT, class RhsUnitT>
constexpr inline bool operator!=(Value<LhsUnitT> lhs, Value<RhsUnitT> rhs) noexcept {
	return !(lhs == rhs);
}

template <class LhsUnitT, class RhsUnitT>
constexpr inline auto operator*(Value<LhsUnitT> lhs, Value<RhsUnitT> rhs) noexcept {
	using TargetUnit = LhsUnitT::template MultipliedBy_t<RhsUnitT>;
	return detail::makeValue<TargetUnit>(
		lhs.template value<LhsUnitT>() * rhs.template value<RhsUnitT>());
}

template <class LhsUnitT, class RhsUnitT>
constexpr inline auto operator/(Value<LhsUnitT> lhs, Value<RhsUnitT> rhs) noexcept {
	using TargetUnit = LhsUnitT::template DividedBy_t<RhsUnitT>;
	return detail::makeValue<TargetUnit>(
		lhs.template value<LhsUnitT>() / rhs.template value<RhsUnitT>());
}

template <class UnitT>
constexpr inline auto operator*(Value<UnitT> value, float f) noexcept {
	return Value<UnitT>(value) *= f;
}

template <class UnitT>
constexpr inline auto operator*(float f, Value<UnitT> value) noexcept {
	return Value<UnitT>(value) *= f;
}

template <class UnitT>
constexpr inline auto operator/(Value<UnitT> value, float f) noexcept {
	return Value<UnitT>(value) /= f;
}

template <class UnitT>
constexpr inline auto operator/(float f, Value<UnitT> value) noexcept {
	return Value<UnitT>(value) /= f;
}

template <class UnitT>
inline std::string to_string(Value<UnitT> value) {
	using std::to_string;
	std::ostringstream oss{};
	oss << value.template value<UnitT>() << ' ' << to_string(UnitT());
	return oss.str();
}

template <class UnitT>
inline std::ostream& operator<<(std::ostream& os, Value<UnitT> value) {
	return os << to_string(value);
}

} // namespace detail

// -- base SI units and common multipliers

// ---- length

using Millimetre = detail::LengthUnit<std::milli>;
using Centimetre = detail::LengthUnit<std::centi>;
using Metre = detail::LengthUnit<std::ratio<1>>;
using Kilometre = detail::LengthUnit<std::kilo>;

using Length = detail::Value<Metre>;

constexpr inline auto operator""_m(long double m) noexcept {
	return Length(static_cast<float>(m), Metre());
}

constexpr inline auto operator""_m(unsigned long long m) noexcept {
	return Length(static_cast<float>(m), Metre());
}

constexpr inline auto operator""_km(long double km) noexcept {
	return detail::makeValue<Kilometre>(static_cast<float>(km));
}

constexpr inline auto operator""_km(unsigned long long km) noexcept {
	return detail::makeValue<Kilometre>(static_cast<float>(km));
}

// ---- mass

using Gram = detail::MassUnit<std::milli>;
using Kilogram = detail::MassUnit<std::ratio<1>>;

using Mass = detail::Value<Kilogram>;

constexpr inline auto operator""_g(long double g) noexcept {
	return detail::makeValue<Gram>(static_cast<float>(g));
}

constexpr inline auto operator""_g(unsigned long long int g) noexcept {
	return detail::makeValue<Gram>(static_cast<float>(g));
}

constexpr inline auto operator""_kg(long double kg) noexcept {
	return Mass(static_cast<float>(kg), Kilogram());
}

constexpr inline auto operator""_kg(unsigned long long int kg) noexcept {
	return Mass(static_cast<float>(kg), Kilogram());
}

using Microsecond = detail::TimeUnit<std::micro>;
using Millisecond = detail::TimeUnit<std::milli>;
using Second = detail::TimeUnit<std::ratio<1>>;
using Minute = detail::TimeUnit<std::ratio<60>>;
using Hour = detail::TimeUnit<std::ratio<3600>>;

// ---- time

using Time = detail::Value<Second>;

constexpr inline auto operator""_us(long double us) noexcept {
	return Time(static_cast<float>(us), Second());
}

constexpr inline auto operator""_us(unsigned long long us) noexcept {
	return Time(static_cast<float>(us), Second());
}

constexpr inline auto operator""_ms(long double ms) noexcept {
	return Time(static_cast<float>(ms), Second());
}

constexpr inline auto operator""_ms(unsigned long long ms) noexcept {
	return Time(static_cast<float>(ms), Second());
}

constexpr inline auto operator""_s(long double s) noexcept {
	return Time(static_cast<float>(s), Second());
}

constexpr inline auto operator""_s(unsigned long long s) noexcept {
	return Time(static_cast<float>(s), Second());
}

constexpr inline auto operator""_min(long double min) noexcept {
	return Time(static_cast<float>(min), Second());
}

constexpr inline auto operator""_min(unsigned long long min) noexcept {
	return Time(static_cast<float>(min), Second());
}

constexpr inline auto operator""_h(long double h) noexcept {
	return detail::makeValue<Hour>(static_cast<float>(h));
}

constexpr inline auto operator""_h(unsigned long long h) noexcept {
	return detail::makeValue<Hour>(static_cast<float>(h));
}

// -- derived SI units / values

// ---- speed

using Speed = decltype(1_m / 1_s);

using MetresPerSecond = Speed::Unit;
using KilometresPerHour = decltype(1_km / 1_h)::Unit;

constexpr inline auto operator""_mps(long double mps) noexcept {
	return Speed(static_cast<float>(mps), MetresPerSecond());
}

constexpr inline auto operator""_mps(unsigned long long mps) noexcept {
	return Speed(static_cast<float>(mps), MetresPerSecond());
}

constexpr inline auto operator""_kph(long double kph) noexcept {
	return detail::makeValue<KilometresPerHour>(static_cast<float>(kph));
}

constexpr inline auto operator""_kph(unsigned long long kph) noexcept {
	return detail::makeValue<KilometresPerHour>(static_cast<float>(kph));
}

using Acceleration = decltype(std::declval<Speed>() / 1.0_s);
using MetresPerSecondSq = Acceleration::Unit;

using Jerk = decltype(std::declval<Acceleration>() / 1.0_s);
using MetresPerSecondCu = Jerk::Unit;

using Force = decltype(1.0_kg * std::declval<Acceleration>());
using Newtons = Force::Unit;

constexpr inline auto operator""_N(long double n) noexcept {
	return Force(static_cast<float>(n), Newtons());
}

constexpr inline auto operator""_N(unsigned long long n) noexcept {
	return Force(static_cast<float>(n), Newtons());
}

} // namespace dormouse_engine::math

namespace dormouse_engine {

using math::Metre;
using math::Kilogram;
using math::Second;

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
using math::operator""_kph;

using math::operator""_N;

} // namespace math_literals

} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_MATH_SI_HPP_ */
