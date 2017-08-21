#ifndef _DORMOUSEENGINE_RENDERER_CAMERA_HPP_
#define _DORMOUSEENGINE_RENDERER_CAMERA_HPP_

#include "dormouse-engine/math/Matrix.hpp"
#include "dormouse-engine/essentials/smart-pointer-definitions.hpp"

namespace dormouse_engine::renderer {

class Camera {
public:

	virtual const Matrix4x4& viewTransform() const = 0;

	virtual const Vec3& position() const = 0;

protected:

	virtual ~Camera() {
	}

};

DE_SMART_POINTER_DEFINITONS(Camera);

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_CAMERA_HPP_ */
