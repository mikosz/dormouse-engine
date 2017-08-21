#include "OrthographicLens.hpp"

#include "dormouse-engine/milk/graphics/Renderer.hpp"
#include "dormouse-engine/math/Transform.hpp"

#include <functional>

using namespace dormouse_engine;
using namespace dormouse_engine;
using namespace dormouse_engine::renderer;

OrthographicLens::OrthographicLens(
	math::Handedness handedness,
	float viewWidth,
	float viewHeight,
	float nearZ,
	float farZ
	) :
	projectionMatrix_(Transform::orthographicProjection(
		handedness,
		-viewWidth * 0.5f,
		viewWidth * 0.5f, 
		-viewHeight * 0.5f,
		viewHeight * 0.5f,
		nearZ,
		farZ,
		milk::graphics::Renderer::NDC_NEAR
		).matrix())
{
}

const Matrix4x4& OrthographicLens::projectionTransformation() const {
	return projectionMatrix_;
}
