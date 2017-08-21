#ifndef _DORMOUSEENGINE_MATH_FRUSTUM_HPP_
#define _DORMOUSEENGINE_MATH_FRUSTUM_HPP_

#include <array>

#include "Plane.hpp"
#include "Transform.hpp"
#include "Vector.hpp"

namespace coconut::pulp::math {

class Frustum {
public:

	Frustum(Handedness handedness, float focalLength, float aspectRatio, float near, float far) noexcept;

	Frustum(const Transform& projectionTransform) noexcept;

	bool contains(const Vec3& point) const noexcept;

private:

	enum class Side {
		NEAR,
		FAR,
		LEFT,
		RIGHT,
		BOTTOM,
		TOP
	};

	std::array<Plane, 6> planes_;

};

} // namespace math

using math::Frustum;

} // namespace coconut::pulp

#endif /* _DORMOUSEENGINE_MATH_FRUSTUM_HPP_ */
