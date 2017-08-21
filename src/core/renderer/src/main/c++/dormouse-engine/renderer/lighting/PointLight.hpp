#ifndef _DORMOUSEENGINE_RENDERER_LIGHTING_POINTLIGHT_HPP_
#define _DORMOUSEENGINE_RENDERER_LIGHTING_POINTLIGHT_HPP_

#include "dormouse-engine/math/Vector.hpp"

namespace dormouse_engine::renderer::lighting {

class PointLight {
public:

	PointLight(
		const math::Vec3& position,
		const math::Vec3& attenuation, // <constant, linear, quadratic>
		const math::Vec4& ambientColour,
		const math::Vec4& diffuseColour,
		const math::Vec4& specularColour
		) :
		position_(position),
		attenuation_(attenuation),
		ambientColour_(ambientColour),
		diffuseColour_(diffuseColour),
		specularColour_(specularColour)
	{
	}

	const math::Vec3& position() const {
		return position_;
	}

	const math::Vec3& attenuation() const {
		return attenuation_;
	}

	const math::Vec4& ambientColour() const {
		return ambientColour_;
	}

	const math::Vec4& diffuseColour() const {
		return diffuseColour_;
	}

	const math::Vec4& specularColour() const {
		return specularColour_;
	}

private:

	math::Vec3 position_;

	math::Vec3 attenuation_;

	math::Vec4 ambientColour_;

	math::Vec4 diffuseColour_;

	math::Vec4 specularColour_;

};

} // namespace dormouse_engine::renderer::lighting

#endif /* _DORMOUSEENGINE_RENDERER_LIGHTING_POINTLIGHT_HPP_ */
