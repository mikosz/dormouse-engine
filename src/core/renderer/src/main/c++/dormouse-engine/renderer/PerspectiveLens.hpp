#ifndef _DORMOUSEENGINE_RENDERER_PERSPECTIVELENS_HPP_
#define _DORMOUSEENGINE_RENDERER_PERSPECTIVELENS_HPP_

#include "Lens.hpp"

namespace coconut::pulp::renderer {

class PerspectiveLens : public Lens {
public:

	PerspectiveLens(
		pulp::math::Handedness handedness,
		Angle fov,
		float aspectRatio,
		float nearZ,
		float farZ
		);

	const Matrix4x4& projectionTransformation() const override;

private:

	Matrix4x4 projectionMatrix_;

};

} // namespace coconut::pulp::renderer

#endif /* _DORMOUSEENGINE_RENDERER_PERSPECTIVELENS_HPP_ */
