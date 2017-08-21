#ifndef _DORMOUSEENGINE_RENDERER_ORTHOGRAPHIC_HPP_
#define _DORMOUSEENGINE_RENDERER_ORTHOGRAPHIC_HPP_

#include "Lens.hpp"

namespace coconut::pulp::renderer {

class OrthographicLens : public Lens {
public:

	OrthographicLens(
		pulp::math::Handedness handedness,
		float viewWidth,
		float viewHeight,
		float nearZ,
		float farZ
		);

	const Matrix4x4& projectionTransformation() const override;

private:

	Matrix4x4 projectionMatrix_;

};

} // namespace coconut::pulp::renderer

#endif /* _DORMOUSEENGINE_RENDERER_ORTHOGRAPHIC_HPP_ */
