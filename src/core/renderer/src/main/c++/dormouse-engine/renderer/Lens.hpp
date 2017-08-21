#ifndef _DORMOUSEENGINE_RENDERER_LENS_HPP_
#define _DORMOUSEENGINE_RENDERER_LENS_HPP_

#include "coconut/pulp/math/Matrix.hpp"
#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut::pulp::renderer {

class Lens {
public:

	virtual const Matrix4x4& projectionTransformation() const = 0;

protected:

	virtual ~Lens() {
	}

};

CT_MAKE_POINTER_DEFINITIONS(Lens);

} // namespace coconut::pulp::renderer

#endif /* _DORMOUSEENGINE_RENDERER_LENS_HPP_ */
