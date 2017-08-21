#ifndef _DORMOUSEENGINE_RENDERER_CAMERA_HPP_
#define _DORMOUSEENGINE_RENDERER_CAMERA_HPP_

#include "dormouse-engine/math/Matrix.hpp"
#include "dormouse-engine/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace dormouse_engine::renderer {

class Camera {
public:

	virtual const Matrix4x4& viewTransform() const = 0;

	virtual const Vec3& position() const = 0;

protected:

	virtual ~Camera() {
	}

};

CT_MAKE_POINTER_DEFINITIONS(Camera);

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_CAMERA_HPP_ */
