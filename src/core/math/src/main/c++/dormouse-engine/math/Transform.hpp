#ifndef _DORMOUSEENGINE_MATH_TRANSFORM_HPP_
#define _DORMOUSEENGINE_MATH_TRANSFORM_HPP_

#include "dormouse-engine/essentials/Null.hpp"
#include "homogeneous.hpp"
#include "Matrix.hpp"
#include "basis.hpp"

namespace dormouse_engine::math {

namespace detail {

class UncheckedTransform {
public:

	static UncheckedTransform orthographicProjection(
		Handedness handedness,
		float left,
		float right,
		float top,
		float bottom,
		float near,
		float far,
		float ndcNear
	) noexcept;

	static UncheckedTransform perspectiveProjection(
		Handedness handedness,
		float focalLength,
		float aspectRatio,
		float near,
		float far,
		float ndcNear
	) noexcept;

	static UncheckedTransform perspectiveProjection(
		Handedness handedness,
		Angle horizontalFOV,
		float aspectRatio,
		float near,
		float far,
		float ndcNear
	) noexcept;

	static UncheckedTransform translation(const Vec3& vector) noexcept;

	static UncheckedTransform scale(const Vec3& by) noexcept;

	static UncheckedTransform rotation(Vec3 around, Angle by) noexcept;

	UncheckedTransform() noexcept = default;

	UncheckedTransform(Matrix4x4 transformationMatrix) noexcept :
	matrix_(std::move(transformationMatrix))
	{
	}

	//HomogeneousCoordinates apply(const HomogeneousCoordinates& coords) const noexcept {
	//	return Vec4(
	//		matrix_.row(0).dot(coords),
	//		matrix_.row(1).dot(coords),
	//		matrix_.row(2).dot(coords),
	//		matrix_.row(3).dot(coords)
	//	);
	//}

	UncheckedTransform& append(const UncheckedTransform& next) noexcept {
		matrix_ = next.matrix_ * matrix_;
		return *this;
	}

	UncheckedTransform& append(const Matrix4x4& next) noexcept {
		matrix_ = next * matrix_;
		return *this;
	}

	UncheckedTransform then(const UncheckedTransform& next) const noexcept {
		return UncheckedTransform(*this).append(next);
	}

	UncheckedTransform then(const Matrix4x4& next) const noexcept {
		return UncheckedTransform(*this).append(next);
	}

	const Matrix4x4& matrix() const {
		return matrix_;
	}

private:

	Matrix4x4 matrix_ = Matrix4x4::IDENTITY;

};

inline UncheckedTransform operator<<(const UncheckedTransform& first, const UncheckedTransform& second) {
	return first.then(second);
}

} // namespace detail

// TODO: move below to detail (until MatchingBasis which is to be user-specialised

/**
 * TransformTargetBasis_t is the target type of a transform, respecting Any and Same.
 */
template <class SourceBasisT, class TargetBasisT>
struct TransformTargetBasis {
	using Type = TargetBasisT;
};

template <class SourceBasisT>
struct TransformTargetBasis<SourceBasisT, basis::Same> {
	using Type = SourceBasisT;
};

template <class SourceBasisT, class TargetBasisT>
using TransformTargetBasis_t = typename TransformTargetBasis<SourceBasisT, TargetBasisT>::Type;

/**
 * DecayIfSame_t yields Same if Source and Target are the same, Target otherwise.
 */
template <class SourceBasisT, class TargetBasisT>
struct DecayIfSameBasis {
	using Type = TargetBasisT;
};

template <class SourceBasisT>
struct DecayIfSameBasis<SourceBasisT, SourceBasisT> {
	using Type = basis::Same;
};

template <class SourceBasisT, class TargetBasisT>
using DecayIfSameBasis_t = typename DecayIfSameBasis<SourceBasisT, TargetBasisT>::Type;

/**
 * JointTransformSourceBasis_t is the source type of the transformation created from joining
 * two transforms. Does not respect Same (should be repeated source type).
 */
template <class LhsSourceBasisT, class LhsTargetBasisT, class RhsSourceBasisT, class RhsTargetBasisT>
struct JointTransformSourceBasis {
	using Type = LhsSourceBasisT;
};

template <class RhsSourceBasisT, class RhsTargetBasisT>
struct JointTransformSourceBasis<basis::Any, basis::Any, RhsSourceBasisT, RhsTargetBasisT> {
	using Type = RhsSourceBasisT;
};

template <class LhsSourceBasisT, class LhsTargetBasisT, class RhsSourceBasisT, class RhsTargetBasisT>
using JointTransformSourceBasis_t =
	typename JointTransformSourceBasis<LhsSourceBasisT, LhsTargetBasisT, RhsSourceBasisT, RhsTargetBasisT>::Type;

/**
 * JointTransformTargetBasis_t is the target type of the transformation created from joining
 * two transforms. Does not respect Same (should Be repeated source type).
 */
template <class LhsSourceBasisT, class LhsTargetBasisT, class RhsSourceBasisT, class RhsTargetBasisT>
struct JointTransformTargetBasis {
	using Type = RhsTargetBasisT;
};

template <class LhsSourceBasisT, class LhsTargetBasisT>
struct JointTransformTargetBasis<LhsSourceBasisT, LhsTargetBasisT, basis::Any, basis::Any> {
	using Type = LhsTargetBasisT;
};

template <class LhsSourceBasisT, class LhsTargetBasisT, class RhsSourceBasisT, class RhsTargetBasisT>
using JointTransformTargetBasis_t =
	typename JointTransformTargetBasis<LhsSourceBasisT, LhsTargetBasisT, RhsSourceBasisT, RhsTargetBasisT>::Type;

/**
 * MatchingBasis_v yields true iff the target of a transform is compatible with the source of another.
 */
template <class LhsBasisT, class RhsBasisT>
struct MatchingBasis {
	static constexpr auto value = std::is_same_v<LhsBasisT, RhsBasisT>;
};

template <class LhsBasisT>
struct MatchingBasis<LhsBasisT, basis::Any> {
	static constexpr auto value = true;
};

template <class LhsBasisT, class RhsBasisT>
constexpr auto MatchingBasis_v = MatchingBasis<LhsBasisT, RhsBasisT>::value;

template <class SourceBasisT, class TargetBasisT>
class Transform : SourceBasisT, TargetBasisT {
public:

	static_assert(!std::is_same_v<SourceBasisT, basis::Same>);
	static_assert(!std::is_same_v<TargetBasisT, basis::Any>);
	static_assert(!std::is_same_v<SourceBasisT, basis::Any> || std::is_same_v<TargetBasisT, basis::Same>);

	using SourceBasis = SourceBasisT;
	using TargetBasis = TransformTargetBasis_t<SourceBasis, TargetBasisT>;

	/**
	 * Creates a perspective projection transformation. Positions within the view frustum are mapped into
	 * the homogeneous clip-space <[-1, 1], [-1, 1], [ndcNear, 1], 1>
	 */
	static Transform orthographicProjection(
		Handedness handedness,
		float left,
		float right,
		float top,
		float bottom,
		float near,
		float far,
		float ndcNear
		) noexcept
	{
		return detail::Transform::orthographicProjection(handedness, left, right, top, bottom, near, far, ndcNear);
	}

	/**
	 * Creates a perspective projection transformation. Positions within the view frustum are mapped into
	 * the homogeneous clip-space <[-1, 1], [-1, 1], [ndcNear, 1], 1> * w
	 *
	 * @param handedness - handedness of the camera's coordinate system
	 * @param focalLength - distance from the camera to the projection plane
	 * @param aspectRatio - screen height divided by screen width
	 * @param near - distance to the near clipping plane
	 * @param far - distance to the far clipping plane
	 * @param ndcNear - normalised device coordinates near value (this will be -1 for OpenGL and 0 for DirectX)
	 */
	static Transform perspectiveProjection(
		Handedness handedness,
		float focalLength,
		float aspectRatio,
		float near,
		float far,
		float ndcNear
		) noexcept
	{
		return detail::Transform::perspectiveProjection(handedness, focalLength, aspectRatio, near, far, ndcNear);
	}

	/**
	 * Creates a perspective projection transformation. Positions within the view frustum are mapped into
	 * the homogeneous clip-space <[-1, 1], [-1, 1], [ndcNear, 1], 1> * w
	 *
	 * @param handedness - handedness of the camera's coordinate system
	 * @param horizontalFOV - horizontal field-of-view angle
	 * @param aspectRatio - screen height divided by screen width
	 * @param near - distance to the near clipping plane
	 * @param far - distance to the far clipping plane
	 * @param ndcNear - normalised device coordinates near value (this will be -1 for OpenGL and 0 for DirectX)
	 */
	static Transform perspectiveProjection(
		Handedness handedness,
		Angle horizontalFOV,
		float aspectRatio,
		float near,
		float far,
		float ndcNear
		) noexcept
	{
		return detail::UncheckedTransform::perspectiveProjection(handedness, horizontalFOV, aspectRatio, near, far, ndcNear);
	}

	static Transform translation(const Vec3& vector) noexcept {
		return detail::UncheckedTransform::translation(vector);
	}

	static Transform scale(const Vec3& by) noexcept {
		return detail::UncheckedTransform::scale(by);
	}

	static Transform rotation(Vec3 around, Angle by) noexcept {
		return detail::UncheckedTransform::rotation(around, by);
	}

	Transform() = default;

	Transform(Matrix4x4 transformationMatrix) noexcept :
		unchecked_(std::move(transformationMatrix))
	{
	}

	template <class CoordsBasis>
	auto apply(const HomogeneousCoordinates<CoordsBasis>& coords) const noexcept {
		static_assert(
			MatchingBasis_v<SourceBasis, CoordsBasis>,
			"Attempted to apply transform from a non-matching source-basis"
			);

		return Vec4(
			// TODO: access member
			matrix().row(0).dot(coords),
			matrix().row(1).dot(coords),
			matrix().row(2).dot(coords),
			matrix().row(3).dot(coords)
			);
	}

	template <class NextSourceBasis, class NextTargetBasis>
	Transform& append(const Transform<NextSourceBasis, NextTargetBasis>& next) noexcept {
		static_assert(
			MatchingBasis_v<TargetBasis, decltype(next)::SourceBasis>,
			"Attempted to append a transform with a source-type not matching the current target type."
			);
		static_assert(
			MatchingBasis_v<TargetBasis, decltype(next)::TargetBasis>,
			"Attempted to append a transform with a target-type not equal to the current target type."
			);
		unchecked_.append(next.matrix());
		return *this;
	}

	template <class NextSourceBasisT, class NextTargetBasisT>
	auto then(const Transform<NextSourceBasisT, NextTargetBasisT>& next) const noexcept {
		using NextType = std::decay_t<decltype(next)>;
		using NextSourceBasis = NextType::SourceBasis;
		using NextTargetBasis = NextType::TargetBasis;

		static_assert(
			MatchingBasis_v<TargetBasis, NextType::SourceBasis>,
			"Attempted to append a transform with a source-type not matching the current target type."
			);
		
		using ResultSourceBasis = JointTransformSourceBasis_t<SourceBasis, TargetBasis, NextSourceBasis, NextTargetBasis>;
		using ResultTargetBasis = JointTransformTargetBasis_t<SourceBasis, TargetBasis, NextSourceBasis, NextTargetBasis>;
		return Transform<
			ResultSourceBasis, DecayIfSameBasis_t<ResultSourceBasis, ResultTargetBasis>
			>(next.matrix() * matrix());
	}

	const Matrix4x4& matrix() const {
		return unchecked_.matrix();
	}

private:

	detail::UncheckedTransform unchecked_;

	Transform(detail::UncheckedTransform unchecked) noexcept :
		unchecked_(std::move(unchecked))
	{
	}

};

//static_assert(sizeof(Transform<basis::Any, basis::Same>) == sizeof(Matrix4x4));

template <class LhsSourceBasis, class LhsTargetBasis, class RhsSourceBasis, class RhsTargetBasis>
inline auto operator<<(
	const Transform<LhsSourceBasis, LhsTargetBasis>& first,
	const Transform<RhsSourceBasis, RhsTargetBasis>& second
	)
{
	return first.then(second);
}

} // namespace dormouse_engine::math

namespace dormouse_engine {

using math::Transform;

} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_MATH_TRANSFORM_HPP_ */
