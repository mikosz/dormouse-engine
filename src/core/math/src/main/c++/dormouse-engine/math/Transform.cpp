#include "Transform.hpp"

#include <cassert>

#pragma warning(push, 3)
#	include <ponder/classbuilder.hpp>
#pragma warning(pop)

#include "dormouse-engine/reflection/Object.hpp"
#include "ScalarEqual.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::math;

Transform Transform::orthographicProjection(
	Handedness handedness,
	float left,
	float right,
	float top,
	float bottom,
	float nearDistance,
	float farDistance,
	float ndcNear
	) noexcept
{
	auto matrix = Matrix4x4();
	matrix[0][0] = 2.0f / (right - left);
	matrix[1][1] = 2.0f / (top - bottom);
	if (handedness == Handedness::RIGHT) {
		matrix[2][2] = (1.0f - ndcNear) / (nearDistance - farDistance);
	} else {
		matrix[2][2] = (1.0f - ndcNear) / (farDistance - nearDistance);
	}
	matrix[0][3] = -(right + left) / (right - left);
	matrix[1][3] = -(top + bottom) / (top - bottom);
	matrix[2][3] = (nearDistance - (farDistance * ndcNear)) / (nearDistance - farDistance);
	matrix[3][3] = 1.0f;

	return Transform(matrix);
}

Transform Transform::perspectiveProjection(
	Handedness handedness,
	float focalLength,
	float aspectRatio,
	float nearDistance,
	float farDistance,
	float ndcNear
	) noexcept
{
	assert(nearDistance > 0.0f);
	assert(nearDistance < farDistance);

	auto matrix = Matrix4x4();
	matrix[0][0] = focalLength;
	matrix[1][1] = focalLength / aspectRatio;
	matrix[2][3] = -((1 - ndcNear) * farDistance * nearDistance) / (farDistance - nearDistance);
	if (handedness == Handedness::RIGHT) {
		matrix[2][2] = -((-ndcNear * nearDistance) + farDistance) / (farDistance - nearDistance);
		matrix[3][2] = -1.0f;
	} else {
		matrix[2][2] = ((-ndcNear * nearDistance) + farDistance) / (farDistance - nearDistance);
		matrix[3][2] = 1.0f;
	}

	return Transform(matrix);
}

Transform Transform::perspectiveProjection(
	Handedness handedness,
	Angle horizontalFOV,
	float aspectRatio,
	float nearDistance,
	float farDistance,
	float ndcNear
	) noexcept
{
	const auto focalLength = 1.0f / std::tan(horizontalFOV.radians() / 2.0f);

	return perspectiveProjection(handedness, focalLength, aspectRatio, nearDistance, farDistance, ndcNear);
}

Transform Transform::translation(const Vec3& vector) noexcept {
	auto matrix = Matrix4x4::IDENTITY;
	matrix[0][3] = vector.x();
	matrix[1][3] = vector.y();
	matrix[2][3] = vector.z();

	return Transform(matrix);
}

Transform Transform::scale(const Vec3& by) noexcept {
	auto matrix = Matrix4x4::IDENTITY;
	matrix[0][0] = by.x();
	matrix[1][1] = by.y();
	matrix[2][2] = by.z();

	return Transform(matrix);
}

Transform Transform::rotation(Vec3 around, Angle by) noexcept {
	around.normalise();

	const auto x = around.x();
	const auto y = around.y();
	const auto z = around.z();
	const auto xSq = x * x;
	const auto ySq = y * y;
	const auto zSq = z * z;
	const auto xy = x * y;
	const auto xz = x * z;
	const auto yz = y * z;
	const auto cos = std::cos(by.radians());
	const auto sin = std::sin(by.radians());

	auto matrix = Matrix4x4();
	matrix[0][0] = xSq + (1.0f - xSq) * cos;
	matrix[1][0] = xy * (1.0f - cos) + z * sin;
	matrix[2][0] = xz * (1.0f - cos) - y * sin;
	matrix[0][1] = xy * (1.0f - cos) - z * sin;
	matrix[1][1] = ySq + (1.0f - ySq) * cos;
	matrix[2][1] = yz * (1.0f - cos) + x * sin;
	matrix[0][2] = xz * (1.0f - cos) + y * sin;
	matrix[1][2] = yz * (1.0f - cos) - x * sin;
	matrix[2][2] = zSq + (1.0f - zSq) * cos;
	matrix[3][3] = 1.0f;

	return Transform(matrix);
}

void detail::declareTransform() {
	ponder::Class::declare<Transform>("dormouse_engine::math::Transform")
		.property("matrix", &Transform::matrix)
		;
}
