#include "Frustum.hpp"

#include <algorithm>
#include <cmath>

#include "Transform.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine;
using namespace dormouse_engine::math;

Frustum::Frustum(Handedness handedness, float focalLength, float aspectRatio, float near, float far) noexcept
// TODO: temp workaround
	: Frustum(detail::UncheckedTransform::perspectiveProjection(handedness, focalLength, aspectRatio, near, far, -1.0f).matrix())
{
}

Frustum::Frustum(const Matrix4x4& projectionMatrix) noexcept :
	planes_{
		Plane(projectionMatrix.row(3) + projectionMatrix.row(2)),
		Plane(projectionMatrix.row(3) - projectionMatrix.row(2)),
		Plane(projectionMatrix.row(3) + projectionMatrix.row(0)),
		Plane(projectionMatrix.row(3) - projectionMatrix.row(0)),
		Plane(projectionMatrix.row(3) + projectionMatrix.row(1)),
		Plane(projectionMatrix.row(3) - projectionMatrix.row(1)),
	}
{
}

bool Frustum::contains(const Vec3& point) const noexcept {
	return std::all_of(planes_.begin(), planes_.end(), [&point](const auto& plane) {
			return plane.signedDistanceToPoint(point) >= 0.0f;
		});
}
