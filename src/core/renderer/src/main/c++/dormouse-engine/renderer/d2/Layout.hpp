#ifndef _DORMOUSEENGINE_RENDERER_D2_LAYOUT_HPP_
#define _DORMOUSEENGINE_RENDERER_D2_LAYOUT_HPP_

#include <utility>

#include "dormouse-engine/enums/enum.hpp"
#include "dormouse-engine/math/Transform.hpp"
#include "Dimension.hpp"

namespace dormouse_engine::renderer::d2 {

class Layout final {
public:

	DE_MEMBER_ENUM(
		HorizontalAnchor,
		(LEFT)
		(CENTRE)
		(RIGHT)
	);

	DE_MEMBER_ENUM(
		VerticalAnchor,
		(TOP)
		(MIDDLE)
		(BOTTOM)
		);

	using Anchor = std::tuple<HorizontalAnchor, VerticalAnchor>;

	Layout() :
		anchor_(HorizontalAnchor::CENTRE, VerticalAnchor::MIDDLE),
		positionX_(Ndc(0.0f)),
		positionY_(Ndc(0.0f)),
		width_(Ndc(1.0f)),
		height_(Ndc(1.0f))
	{
	}

	Layout(Anchor anchor, Dimension positionX, Dimension positionY, Dimension width, Dimension height) noexcept :
		anchor_(anchor),
		positionX_(positionX),
		positionY_(positionY),
		width_(width),
		height_(height)
	{
	}

	const math::Transform toNDC() const noexcept {
		// TODO: anchor
		return
			math::Transform::scale({ width_.ndc(), height_.ndc(), 1.0f }) <<
			math::Transform::translation({ positionX_.ndc(), positionY_.ndc(), 0.0f })
			;
	}

	const Anchor anchor() const noexcept {
		return anchor_;
	}

	Anchor& anchor() noexcept {
		return anchor_;
	}

	const Dimension& positionX() const noexcept {
		return positionX_;
	}

	Dimension& positionX() noexcept {
		return positionX_;
	}

	const Dimension& positionY() const noexcept {
		return positionY_;
	}

	Dimension& positionY() noexcept {
		return positionY_;
	}

	const Dimension& width() const noexcept {
		return width_;
	}

	Dimension& width() noexcept {
		return width_;
	}

	const Dimension& height() const noexcept {
		return height_;
	}

	Dimension& height() noexcept {
		return height_;
	}

private:

	Anchor anchor_;

	Dimension positionX_;

	Dimension positionY_;

	Dimension width_;

	Dimension height_;

};

} // namespace dormouse_engine::renderer::d2

#endif /* _DORMOUSEENGINE_RENDERER_D2_LAYOUT_HPP_ */
