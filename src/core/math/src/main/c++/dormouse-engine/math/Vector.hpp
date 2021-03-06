#ifndef _DORMOUSEENGINE_MATH_VECTOR_HPP_
#define _DORMOUSEENGINE_MATH_VECTOR_HPP_

#include <cmath>
#include <cassert>
#include <array>
#include <algorithm>
#include <functional>
#include <memory>
#include <numeric>
#include <iosfwd>
#include <initializer_list>
#include <type_traits>

#include <boost/operators.hpp>

#include "dormouse-engine/essentials/InfixOstreamIterator.hpp"
#include "ScalarEqual.hpp"

namespace dormouse_engine::math {

template <class ScalarType, size_t DIMENSIONS_PARAM, class ScalarEqualityFunc = ScalarEqual<ScalarType>>
class Vector :
	boost::equality_comparable<Vector<ScalarType, DIMENSIONS_PARAM, ScalarEqualityFunc>,
	boost::additive<Vector<ScalarType, DIMENSIONS_PARAM, ScalarEqualityFunc>,
	boost::multiplicative<Vector<ScalarType, DIMENSIONS_PARAM, ScalarEqualityFunc>, ScalarType
	>>>
{
public:

	using Scalar = ScalarType;

	static const auto DIMENSIONS = DIMENSIONS_PARAM;

	using Elements = std::array<Scalar, DIMENSIONS>;

	// --- CONSTRUCTORS AND OPERATORS

	template <
		class... CompatibleTypes,
		class = std::enable_if_t<sizeof...(CompatibleTypes) == DIMENSIONS || sizeof...(CompatibleTypes) == 0>
		>
	explicit constexpr Vector(CompatibleTypes&&... values) noexcept :
		elements_{ std::forward<CompatibleTypes>(values)... }
	{
		static_assert(sizeof...(values) == DIMENSIONS || sizeof...(values) == 0, "Bad number of arguments");
	}

	template <
		class CompatibleScalarType,
		size_t OTHER_DIMENSIONS,
		class OtherScalarEqualityFunc,
		class... TailTypes,
		class = std::enable_if_t<sizeof...(TailTypes) + OTHER_DIMENSIONS == DIMENSIONS>
		>
	explicit constexpr Vector(
		const Vector<CompatibleScalarType, OTHER_DIMENSIONS, OtherScalarEqualityFunc>& other,
		TailTypes&&... tail
		)
	{
		std::copy(other.elements().begin(), other.elements().end(), elements_.begin());
		setTail_(elements_, OTHER_DIMENSIONS, std::forward<TailTypes>(tail)...);
	}

	// TODO: consider removing std::initializer_list constructor, as the number of values cannot be
	// checked at compile-time
	constexpr Vector(std::initializer_list<Scalar> values) noexcept {
		assert(values.size() == DIMENSIONS || values.size() == 0);
		std::copy(values.begin(), values.end(), elements_.begin());
		std::uninitialized_fill(
			elements_.begin() + values.size(),
			elements_.end(),
			Scalar(0)
			);
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector& vector) {
		os << '<';
		std::copy(vector.elements_.begin(), vector.elements_.end(),
			dormouse_engine::essentials::InfixOstreamIterator<ScalarType>(os, ", "));
		os << '>';
		return os;
	}

	friend bool operator==(const Vector& lhs, const Vector& rhs) noexcept {
		return std::equal(lhs.elements_.begin(), lhs.elements_.end(), rhs.elements_.begin(), ScalarEqualityFunc());
	}

	Vector& operator+=(const Vector& other) noexcept {
		std::transform(elements_.begin(), elements_.end(), other.elements_.begin(), elements_.begin(), std::plus<>());
		return *this;
	}

	Vector& operator-=(const Vector& other) noexcept {
		std::transform(elements_.begin(), elements_.end(), other.elements_.begin(), elements_.begin(), std::minus<>());
		return *this;
	}

	Vector operator-() const noexcept {
		auto result = Vector();
		std::transform(elements_.begin(), elements_.end(), result.elements_.begin(), std::negate<>());
		return result;
	}

	Vector& operator*=(Scalar scalar) noexcept {
		std::transform(elements_.begin(), elements_.end(), elements_.begin(), [scalar](auto element) {
				return element * scalar;
			});
		return *this;
	}

	Vector& operator/=(Scalar scalar) noexcept {
		std::transform(elements_.begin(), elements_.end(), elements_.begin(), [scalar](auto element) {
			return element / scalar;
		});
		return *this;
	}

	// --- CONVERTERS

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	std::enable_if_t<(DIMENSIONS_PARAM_> 2), Vector<Scalar, 3, ScalarEqualityFunc>>
		xyz() const noexcept
	{
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return Vector<Scalar, 3, ScalarEqualityFunc>(x(), y(), z());
	}

	// --- VECTOR-SPECIFIC OPERATIONS

	Scalar dot(const Vector& other) const noexcept {
		return std::inner_product(elements_.begin(), elements_.end(), other.elements_.begin(), Scalar(0));
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	constexpr std::enable_if_t<(DIMENSIONS_PARAM_ == 3), Vector> cross(const Vector& other) const noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return {
			(y() * other.z()) - (z() * other.y()),
			(z() * other.x()) - (x() * other.z()),
			(x() * other.y()) - (y() * other.x())
			};
	}

	Scalar length() const noexcept {
		using std::sqrt;
		return sqrt(lengthSq());
	}

	Scalar lengthSq() const noexcept {
		return dot(*this);
	}

	Vector& normalise() noexcept {
		const auto l = length();
		if (l > Scalar(0)) {
			*this /= l;
		}
		return *this;
	}

	Vector normalised() const noexcept {
		auto result = *this;
		return result.normalise();
	}

	// --- ACCESSORS

	constexpr const Scalar& operator[](size_t index) const noexcept {
		assert(index < DIMENSIONS);
		return elements_[index];
	}

	Scalar& operator[](size_t index) noexcept {
		assert(index < DIMENSIONS);
		return elements_[index];
	}

	template <size_t INDEX>
	constexpr std::enable_if_t<(DIMENSIONS > INDEX), const Scalar&> get() const noexcept {
		return elements_[INDEX];
	}

	template <size_t INDEX>
	std::enable_if_t<(DIMENSIONS > INDEX), Scalar&> get() noexcept {
		return elements_[INDEX];
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	constexpr std::enable_if_t<(DIMENSIONS_PARAM_> 0), const Scalar&> x() const noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<0>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	std::enable_if_t<(DIMENSIONS_PARAM_> 0), Scalar&> x() noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<0>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	constexpr std::enable_if_t<(DIMENSIONS_PARAM_> 1), const Scalar&> y() const noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<1>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	std::enable_if_t<(DIMENSIONS_PARAM_> 1), Scalar&> y() noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<1>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	constexpr std::enable_if_t<(DIMENSIONS_PARAM_ > 2), const Scalar&> z() const noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<2>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	std::enable_if_t<(DIMENSIONS_PARAM_> 2), Scalar&> z() noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<2>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	constexpr std::enable_if_t<(DIMENSIONS_PARAM_> 3), const Scalar&> w() const noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<3>();
	}

	template <size_t DIMENSIONS_PARAM_ = DIMENSIONS_PARAM>
	std::enable_if_t<(DIMENSIONS_PARAM_> 3), Scalar&> w() noexcept {
		static_assert(DIMENSIONS_PARAM_ == DIMENSIONS_PARAM, "Dimensions changed");
		return get<3>();
	}

	constexpr const Elements& elements() const noexcept {
		return elements_;
	}

private:

	Elements elements_;

	template <class HeadType, class... TailTypes>
	static constexpr void setTail_(
		Elements& elems,
		size_t index,
		HeadType&& head,
		TailTypes&&... tailTypes
		) noexcept
	{
		elems[index] = std::forward<HeadType>(head);
		setTail_(elems, index + 1, std::forward<TailTypes>(tailTypes)...);
	}

	static constexpr void setTail_(Elements&, size_t) noexcept {
	}

};

template <class S, size_t D, class SEF>
S dot(const Vector<S, D, SEF>& lhs, const Vector<S, D, SEF>& rhs) noexcept {
	return lhs.dot(rhs);
}

template <class S, size_t D, class SEF>
constexpr std::enable_if_t<D == 3, Vector<S, D, SEF>>
	cross(const Vector<S, D, SEF>& lhs, const Vector<S, D, SEF>& rhs) noexcept
{
	return lhs.cross(rhs);
}

using Vec2 = Vector<float, 2>;
static_assert(sizeof(Vec2) == sizeof(float) * 2, "Empty base optimisation didn't work");
static_assert(std::is_trivially_copyable<Vec2>::value, "Vector is not trivially copiable");

using Vec3 = Vector<float, 3>;
static_assert(sizeof(Vec3) == sizeof(float) * 3, "Empty base optimisation didn't work");
static_assert(std::is_trivially_copyable<Vec3>::value, "Vector is not trivially copiable");

using Vec4 = Vector<float, 4>;
static_assert(sizeof(Vec4) == sizeof(float) * 4, "Empty base optimisation didn't work");
static_assert(std::is_trivially_copyable<Vec4>::value, "Vector is not trivially copiable");

} // namespace dormouse_engine::math

namespace dormouse_engine {

// TODO: re-enable when primitive::Vector is removed
// using math::Vector;
using math::Vec2;
using math::Vec3;
using math::Vec4;

} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_MATH_VECTOR_HPP_ */
