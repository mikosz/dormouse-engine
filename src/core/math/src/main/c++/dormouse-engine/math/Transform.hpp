#ifndef _DORMOUSEENGINE_MATH_TRANSFORM_HPP_
#define _DORMOUSEENGINE_MATH_TRANSFORM_HPP_

#pragma warning(push, 3)
#	include <ponder/pondertype.hpp>
#pragma warning(pop)

#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/graphics/ShaderDataType.hpp"
#include "homogeneous.hpp"
#include "Matrix.hpp"

namespace dormouse_engine::math {

class Transform {
public:

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
		) noexcept;

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
		) noexcept;

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
		) noexcept;

	static Transform translation(const Vec3& vector) noexcept;

	static Transform scale(const Vec3& by) noexcept;

	static Transform rotation(Vec3 around, Angle by) noexcept;

	Transform() noexcept = default;

	Transform(Matrix4x4 transformationMatrix) noexcept :
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

	Transform& append(const Transform& next) noexcept {
		matrix_ = next.matrix_ * matrix_;
		return *this;
	}

	Transform then(const Transform& next) const noexcept {
		return Transform(*this).append(next);
	}

	const Matrix4x4& matrix() const {
		return matrix_;
	}

	void writeShaderData(essentials::BufferView buffer, graphics::ShaderDataType dataType) const {
		matrix_.writeShaderData(buffer, dataType);
	}

private:

	Matrix4x4 matrix_ = Matrix4x4::IDENTITY;

};

inline Transform operator<<(const Transform& first, const Transform& second) {
	return first.then(second);
}

namespace detail { void declareTransform(); }

} // namespace dormouse_engine::math

namespace dormouse_engine {

using math::Transform;

} // namespace dormouse_engine

PONDER_AUTO_TYPE(
	dormouse_engine::math::Transform,
	&dormouse_engine::math::detail::declareTransform
);

#endif /* _DORMOUSEENGINE_MATH_TRANSFORM_HPP_ */
