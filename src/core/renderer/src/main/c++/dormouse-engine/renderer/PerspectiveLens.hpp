#ifndef _DORMOUSEENGINE_RENDERER_PERSPECTIVELENS_HPP_
#define _DORMOUSEENGINE_RENDERER_PERSPECTIVELENS_HPP_

#include "Lens.hpp"

namespace dormouse_engine::renderer {

class PerspectiveLens : public Lens {
public:

	PerspectiveLens(
		math::Handedness handedness,
		Angle fov,
		float aspectRatio,
		float nearZ,
		float farZ
		);

	const Matrix4x4& projectionTransformation() const override;

private:

	Matrix4x4 projectionMatrix_;

};

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_PERSPECTIVELENS_HPP_ */
