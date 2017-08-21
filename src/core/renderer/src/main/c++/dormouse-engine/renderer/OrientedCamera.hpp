#ifndef _DORMOUSEENGINE_RENDERER_ORIENTEDCAMERA_HPP_
#define _DORMOUSEENGINE_RENDERER_ORIENTEDCAMERA_HPP_

#include "dormouse-engine/math/Transform.hpp"
#include "dormouse-engine/math/Vector.hpp"

#include "dormouse-engine/essentials/Lazy.hpp"

#include "Camera.hpp"

namespace dormouse_engine::renderer {

// TODO: humbug
class OrientedCamera : public Camera {
public:

	OrientedCamera() :
		position_([this](Vec3& position) {
				position = transform_.matrix().transpose().inverse()[3].xyz();
			}) // TODO: check if inverse / transposed needed. Use view.
	{
	}

	const Matrix4x4& viewTransform() const override {
		return transform_.matrix();
	}

	const Vec3& position() const override {
		return position_.get();
	}

	void reset() {
		transform_ = Transform();
		position_.invalidate();
	}

	void translate(const Vec3& translation) {
		transform_.append(Transform::translation(-translation));
		position_.invalidate();
	}

	void rotate(const Vec3& around, Angle by) {
		transform_.append(Transform::rotation(around, -by));
		position_.invalidate();
	}

private:

	Transform transform_;

	milk::utils::Lazy<Vec3> position_;

};

DE_SMART_POINTER_DEFINITONS(OrientedCamera);

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_ORIENTEDCAMERA_HPP_ */
