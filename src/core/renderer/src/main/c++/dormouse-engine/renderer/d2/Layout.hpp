#ifndef _DORMOUSEENGINE_RENDERER_D2_LAYOUT_HPP_
#define _DORMOUSEENGINE_RENDERER_D2_LAYOUT_HPP_

#include "dormouse-engine/enums/enum.hpp"
#include "dormouse-engine/math/Vector.hpp"

namespace dormouse_engine::renderer::d2 {

class Dimension final {
public:

	DE_MEMBER_ENUM(
		Type,
		// Normalised Device Coordinates dimension (a length of 1 is half the width/height of the window)
		(NDC_ABSOLUTE)
		// dimension relative to window extents (0.0 is bottom left, a length of 1.0 is the whole width/height of the window)
		(WINDOW_RELATIVE)
		// window pixels (0 is bottom left, a length of 200 is 200 pixels)
		(PIXELS)
	);

private:



};

class Layout final {
public:

	DE_MEMBER_ENUM(
		Anchor,
		(TOP_LEFT)
		(TOP_CENTRE)
		(TOP_RIGHT)
		(MIDDLE_LEFT)
		(MIDDLE_CENTRE)
		(MIDDLE_RIGHT)
		(BOTTOM_LEFT)
		(BOTTOM_CENTRE)
		(BOTTOM_RIGHT)
		);

	Layout() = default;

	

private:

	Anchor anchor_ = Anchor::MIDDLE_CENTRE;

	Dimension anchorPositionX_;

	Dimension anchorPositionY_;

	Dimension width_;

	Dimension height_;

};

} // namespace dormouse_engine::renderer::d2

#endif /* _DORMOUSEENGINE_RENDERER_D2_LAYOUT_HPP_ */
