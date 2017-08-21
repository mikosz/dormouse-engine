#ifndef _DORMOUSEENGINE_RENDERER_LIGHTING_DIRECTIONALLIGHT_HPP_
#define _DORMOUSEENGINE_RENDERER_LIGHTING_DIRECTIONALLIGHT_HPP_

#include "dormouse-engine/math/Vector.hpp"
#include "dormouse-engine/renderer/shader/ReflectiveObject.hpp"

namespace dormouse_engine::renderer::lighting {

class DirectionalLight {
public:

	DirectionalLight(
		const math::Vec3& direction,
		const math::Vec4& ambientColour,
		const math::Vec4& diffuseColour,
		const math::Vec4& specularColour
		) :
		direction_(direction),
		ambientColour_(ambientColour),
		diffuseColour_(diffuseColour),
		specularColour_(specularColour)
	{
	}

	const math::Vec3& direction() const {
		return direction_;
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

	math::Vec3 direction_;

	math::Vec4 ambientColour_;

	math::Vec4 diffuseColour_;

	math::Vec4 specularColour_;

	friend class renderer::shader::ReflectiveInterface<DirectionalLight>;

};

} // namespace dormouse_engine::renderer::lighting

#endif /* _DORMOUSEENGINE_RENDERER_LIGHTING_DIRECTIONALLIGHT_HPP_ */
