#ifndef _DORMOUSEENGINE_MATH_FRUSTUM_HPP_
#define _DORMOUSEENGINE_MATH_FRUSTUM_HPP_

#include <array>

#include "Plane.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

namespace dormouse_engine::math {

class Frustum {
public:

	Frustum(Handedness handedness, float focalLength, float aspectRatio, float near, float far) noexcept;

	Frustum(const Matrix4x4& projectionMatrix) noexcept;

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

} // namespace dormouse_engine::math

namespace dormouse_engine {

using math::Frustum;

} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_MATH_FRUSTUM_HPP_ */
