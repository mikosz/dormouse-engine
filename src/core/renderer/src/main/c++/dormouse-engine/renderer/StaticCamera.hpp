#ifndef _DORMOUSEENGINE_RENDERER_STATICCAMERA_HPP_
#define _DORMOUSEENGINE_RENDERER_STATICCAMERA_HPP_

#include "Camera.hpp"

namespace coconut::pulp::renderer {

class StaticCamera : public Camera {
public:

	const pulp::math::Matrix& viewTransformation() const override {
		return pulp::math::Matrix::IDENTITY;
	}

};

CT_MAKE_POINTER_DEFINITIONS(StaticCamera);

} // namespace coconut::pulp::renderer

#endif /* _DORMOUSEENGINE_RENDERER_STATICCAMERA_HPP_ */
