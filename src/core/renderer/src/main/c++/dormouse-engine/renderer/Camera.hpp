#ifndef _DORMOUSEENGINE_RENDERER_CAMERA_HPP_
#define _DORMOUSEENGINE_RENDERER_CAMERA_HPP_

#include "coconut/pulp/math/Matrix.hpp"
#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut::pulp::renderer {

class Camera {
public:

	virtual const Matrix4x4& viewTransform() const = 0;

	virtual const Vec3& position() const = 0;

protected:

	virtual ~Camera() {
	}

};

CT_MAKE_POINTER_DEFINITIONS(Camera);

} // namespace coconut::pulp::renderer

#endif /* _DORMOUSEENGINE_RENDERER_CAMERA_HPP_ */
