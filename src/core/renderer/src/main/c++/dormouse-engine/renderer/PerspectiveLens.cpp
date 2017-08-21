#include "PerspectiveLens.hpp"

#include "dormouse-engine/milk/graphics/Renderer.hpp"
#include "dormouse-engine/math/Transform.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine;
using namespace dormouse_engine::renderer;

PerspectiveLens::PerspectiveLens(
	Handedness handedness,
	Angle fov,
	float aspectRatio,
	float nearZ,
	float farZ
	) :
	projectionMatrix_(
		Transform::perspectiveProjection(
			handedness,
			fov,
			aspectRatio,
			nearZ,
			farZ,
			milk::graphics::Renderer::NDC_NEAR
			).matrix()
		)
{
}

const Matrix4x4& PerspectiveLens::projectionTransformation() const {
	return projectionMatrix_;
}
