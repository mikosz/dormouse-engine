#ifndef _DORMOUSEENGINE_RENDERER_STATICCAMERA_HPP_
#define _DORMOUSEENGINE_RENDERER_STATICCAMERA_HPP_

#include "Camera.hpp"

namespace dormouse_engine::renderer {

class StaticCamera : public Camera {
public:

	const math::Matrix& viewTransformation() const override {
		return math::Matrix::IDENTITY;
	}

};

CT_MAKE_POINTER_DEFINITIONS(StaticCamera);

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_STATICCAMERA_HPP_ */
