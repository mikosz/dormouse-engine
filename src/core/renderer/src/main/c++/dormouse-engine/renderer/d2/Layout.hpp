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
		auto centreOffset = math::Vec2();

		switch (std::get<HorizontalAnchor>(anchor_)) {
		case HorizontalAnchor::LEFT:
			centreOffset.x() = width_.ndcDistance() * 0.5f;
			break;
		case HorizontalAnchor::CENTRE:
			break;
		case HorizontalAnchor::RIGHT:
			centreOffset.x() = -width_.ndcDistance() * 0.5f;
			break;
		default:
			assert(!"Invalid horizontal anchor value");
		}

		switch (std::get<VerticalAnchor>(anchor_)) {
		case VerticalAnchor::BOTTOM:
			centreOffset.y() = height_.ndcDistance() * 0.5f;
			break;
		case VerticalAnchor::MIDDLE:
			break;
		case VerticalAnchor::TOP:
			centreOffset.y() = -height_.ndcDistance() * 0.5f;
			break;
		default:
			assert(!"Invalid vertical anchor value");
		}

		return
			math::Transform::scale({ width_.ndcDistance() * 0.5f, height_.ndcDistance() * 0.5f, 1.0f }) <<
			math::Transform::translation(
				{ positionX_.ndcPosition() + centreOffset.x(), positionY_.ndcPosition() + centreOffset.y(), 0.0f }
				)
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
