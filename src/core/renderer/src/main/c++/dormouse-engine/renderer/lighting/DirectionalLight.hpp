#ifndef _DORMOUSEENGINE_RENDERER_LIGHTING_DIRECTIONALLIGHT_HPP_
#define _DORMOUSEENGINE_RENDERER_LIGHTING_DIRECTIONALLIGHT_HPP_

#include "coconut/pulp/math/Vector.hpp"
#include "coconut/pulp/renderer/shader/ReflectiveObject.hpp"

namespace coconut::pulp::renderer::lighting {

class DirectionalLight {
public:

	DirectionalLight(
		const pulp::math::Vec3& direction,
		const pulp::math::Vec4& ambientColour,
		const pulp::math::Vec4& diffuseColour,
		const pulp::math::Vec4& specularColour
		) :
		direction_(direction),
		ambientColour_(ambientColour),
		diffuseColour_(diffuseColour),
		specularColour_(specularColour)
	{
	}

	const pulp::math::Vec3& direction() const {
		return direction_;
	}

	const pulp::math::Vec4& ambientColour() const {
		return ambientColour_;
	}

	const pulp::math::Vec4& diffuseColour() const {
		return diffuseColour_;
	}

	const pulp::math::Vec4& specularColour() const {
		return specularColour_;
	}

private:

	pulp::math::Vec3 direction_;

	pulp::math::Vec4 ambientColour_;

	pulp::math::Vec4 diffuseColour_;

	pulp::math::Vec4 specularColour_;

	friend class renderer::shader::ReflectiveInterface<DirectionalLight>;

};

} // namespace coconut::pulp::renderer::lighting

#endif /* _DORMOUSEENGINE_RENDERER_LIGHTING_DIRECTIONALLIGHT_HPP_ */
