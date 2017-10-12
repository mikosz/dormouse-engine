#ifndef _DORMOUSEENGINE_MATH_ANGLE_HPP_
#define _DORMOUSEENGINE_MATH_ANGLE_HPP_

#include <iosfwd>

#include <boost/operators.hpp>

namespace dormouse_engine::math {

constexpr auto PI = 3.14159264f;

class Angle :
	boost::less_than_comparable<Angle,
	boost::equality_comparable<Angle,
	boost::additive<Angle,
	boost::multiplicative<Angle, float>>>>
{
public:

	static constexpr Angle right() {
		return Angle(PI * 0.5f);
	}

	static constexpr Angle halfFull() {
		return Angle(PI);
	}

	static constexpr Angle full() {
		return Angle(PI * 2.0f);
	}

	constexpr Angle() = default;

	constexpr float radians() const noexcept {
		return radians_;
	}

	constexpr float degrees() const noexcept {
		return radians_ * (180.0f / PI);
	}

	constexpr bool operator==(const Angle& rhs) const noexcept {
		return radians_ == rhs.radians_;
	}

	constexpr bool operator<(const Angle& rhs) const noexcept {
		return radians_ < rhs.radians_;
	}

	constexpr Angle& operator+=(const Angle& rhs) noexcept {
		radians_ += rhs.radians_;
		return *this;
	}

	constexpr Angle& operator-=(const Angle& rhs) noexcept {
		radians_ -= rhs.radians_;
		return *this;
	}

	constexpr Angle& operator*=(float rhs) noexcept {
		radians_ *= rhs;
		return *this;
	}

	constexpr Angle& operator/=(float rhs) noexcept {
		radians_ /= rhs;
		return *this;
	}

	constexpr Angle operator-() const noexcept {
		return Angle(-1.0f * radians_);
	}

	friend constexpr const Angle radians(float r) noexcept;

	friend constexpr const Angle degrees(float d) noexcept;

private:

	constexpr explicit Angle(float r) noexcept :
		radians_(r)
	{
	}

private:

	float radians_;

};

static_assert(sizeof(Angle) == sizeof(float), "Angle should have no extra data");

inline std::ostream& operator<<(std::ostream& os, const Angle& angle) {
	return os << angle.radians() << " rad";
}

inline constexpr const Angle radians(float r) noexcept {
	return Angle(r);
}

inline constexpr const Angle degrees(float d) noexcept {
	return Angle(d * (PI / 180.0f));
}

inline constexpr const Angle operator""_rad(long double r) noexcept {
	return radians(static_cast<float>(r));
}

inline constexpr Angle operator""_deg(long double d) noexcept {
	return degrees(static_cast<float>(d));
}

} // namespace dormouse_engine::math

namespace dormouse_engine {

using math::PI;
using math::Angle;
using math::radians;
using math::degrees;

namespace math_literals {

using math::operator ""_deg;
using math::operator ""_rad;

} // namespace math_literals

} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_MATH_ANGLE_HPP_ */
