#ifndef _DORMOUSEENGINE_MATH_TRANSFORM_HPP_
#define _DORMOUSEENGINE_MATH_TRANSFORM_HPP_

#include "homogeneous.hpp"
#include "Matrix.hpp"

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

	HomogeneousCoordinates apply(const HomogeneousCoordinates& coords) const noexcept {
		return Vec4(
			matrix_.row(0).dot(coords),
			matrix_.row(1).dot(coords),
			matrix_.row(2).dot(coords),
			matrix_.row(3).dot(coords)
			);
	}

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

enum class BasisId {
	ANY,
	SAME,
	SCREEN,
	CAMERA,
	WORLD,
	LOCAL
};

template <BasisId FROM_BASIS, BasisId TO_BASIS>
class Transform {
public:

	static_assert(FROM_BASIS != BasisId::SAME);
	static_assert(TO_BASIS != BasisId::ANY);

	static constexpr auto FROM_BASIS_ID = FROM_BASIS;
	static constexpr auto TO_BASIS_ID = TO_BASIS;

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

	HomogeneousCoordinates apply(const HomogeneousCoordinates& coords) const noexcept {
		return unchecked_.apply(coords);
	}

	template <BasisId NEXT_FROM_BASIS, BasisId NEXT_TO_BASIS>
	Transform& append(const Transform<NEXT_FROM_BASIS, NEXT_TO_BASIS>& next) noexcept {
		static_assert((NEXT_FROM_BASIS == TO_BASIS) || (NEXT_FROM_BASIS == BasisId::ANY), "Incompatible transforms");
		static_assert((NEXT_TO_BASIS == TO_BASIS) || (NEXT_TO_BASIS == BasisId::SAME), "Incompatible transforms");
		unchecked_.append(next.matrix());
		return *this;
	}

	template <BasisId NEXT_FROM_BASIS, BasisId NEXT_TO_BASIS>
	auto then(const Transform<NEXT_FROM_BASIS, NEXT_TO_BASIS>& next) const noexcept {
		static_assert((NEXT_FROM_BASIS == TO_BASIS) || (NEXT_FROM_BASIS == BasisId::ANY), "Incompatible transforms");
		static_assert(NEXT_TO_BASIS == BasisId::CAMERA || NEXT_TO_BASIS == BasisId::SAME);
		constexpr auto RESULT_TO_BASIS = (NEXT_TO_BASIS == BasisId::SAME ? TO_BASIS : NEXT_TO_BASIS);
		return Transform<FROM_BASIS, RESULT_TO_BASIS>(next.matrix() * matrix());
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

template <BasisId LHS_FROM_BASIS, BasisId LHS_TO_BASIS, BasisId RHS_FROM_BASIS, BasisId RHS_TO_BASIS>
inline auto operator<<(
	const Transform<LHS_FROM_BASIS, LHS_TO_BASIS>& first,
	const Transform<RHS_FROM_BASIS, RHS_TO_BASIS>& second
	)
{
	return first.then(second);
}

} // namespace dormouse_engine::math

namespace dormouse_engine {

using math::Transform;

} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_MATH_TRANSFORM_HPP_ */
