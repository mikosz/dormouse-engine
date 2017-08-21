#ifndef _DORMOUSEENGINE_RENDERER_LENS_HPP_
#define _DORMOUSEENGINE_RENDERER_LENS_HPP_

#include "dormouse-engine/math/Matrix.hpp"
#include "dormouse-engine/essentials/smart-pointer-definitions.hpp"

namespace dormouse_engine::renderer {

class Lens {
public:

	virtual const Matrix4x4& projectionTransformation() const = 0;

protected:

	virtual ~Lens() {
	}

};

DE_SMART_POINTER_DEFINITONS(Lens);

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_LENS_HPP_ */
