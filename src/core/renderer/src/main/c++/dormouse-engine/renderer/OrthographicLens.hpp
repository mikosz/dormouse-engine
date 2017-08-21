#ifndef _DORMOUSEENGINE_RENDERER_ORTHOGRAPHIC_HPP_
#define _DORMOUSEENGINE_RENDERER_ORTHOGRAPHIC_HPP_

#include "Lens.hpp"

namespace dormouse_engine::renderer {

class OrthographicLens : public Lens {
public:

	OrthographicLens(
		math::Handedness handedness,
		float viewWidth,
		float viewHeight,
		float nearZ,
		float farZ
		);

	const Matrix4x4& projectionTransformation() const override;

private:

	Matrix4x4 projectionMatrix_;

};

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_ORTHOGRAPHIC_HPP_ */
