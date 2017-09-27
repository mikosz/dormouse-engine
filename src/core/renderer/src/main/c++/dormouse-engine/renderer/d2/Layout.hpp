#ifndef _DORMOUSEENGINE_RENDERER_D2_LAYOUT_HPP_
#define _DORMOUSEENGINE_RENDERER_D2_LAYOUT_HPP_

#include <utility>

#include "dormouse-engine/enums/enum.hpp"
#include "dormouse-engine/math/Transform.hpp"
#include "dormouse-engine/math/Vector.hpp"
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

	using Position = math::Vector<Dimension, 2>;

	using Size = math::Vector<Dimension, 2>;

	Layout() :
		anchor_(HorizontalAnchor::CENTRE, VerticalAnchor::MIDDLE),
		position_(Ndc(0.0f), Ndc(0.0f)),
		size_(Ndc(1.0f), Ndc(1.0f))
	{
	}

	Layout(Anchor anchor, Position position, Size size) noexcept :
		anchor_(std::move(anchor)),
		position_(std::move(position)),
		size_(std::move(size))
	{
	}

	const math::Transform toNDC() const noexcept {
		auto centreOffset = math::Vec2();

		switch (std::get<HorizontalAnchor>(anchor_)) {
		case HorizontalAnchor::LEFT:
			centreOffset.x() = size_.x().ndcVector() * 0.5f;
			break;
		case HorizontalAnchor::CENTRE:
			break;
		case HorizontalAnchor::RIGHT:
			centreOffset.x() = -size_.x().ndcVector() * 0.5f;
			break;
		default:
			assert(!"Invalid horizontal anchor value");
		}

		switch (std::get<VerticalAnchor>(anchor_)) {
		case VerticalAnchor::BOTTOM:
			centreOffset.y() = size_.y().ndcVector() * 0.5f;
			break;
		case VerticalAnchor::MIDDLE:
			break;
		case VerticalAnchor::TOP:
			centreOffset.y() = -size_.y().ndcVector() * 0.5f;
			break;
		default:
			assert(!"Invalid vertical anchor value");
		}

		return
			math::Transform::scale({ size_.x().ndcVector() * 0.5f, size_.y().ndcVector() * 0.5f, 1.0f }) <<
			math::Transform::translation(
				{ position_.x().ndcPoint() + centreOffset.x(), position_.y().ndcPoint() + centreOffset.y(), 0.0f }
				)
			;
	}

	const Anchor anchor() const noexcept {
		return anchor_;
	}

	Anchor& anchor() noexcept {
		return anchor_;
	}

	const Position& position() const noexcept {
		return position_;
	}

	Position& position() noexcept {
		return position_;
	}

	const Size& size() const noexcept {
		return size_;
	}

	Size& size() noexcept {
		return size_;
	}

private:

	Anchor anchor_;

	Position position_;

	Size size_;

};

} // namespace dormouse_engine::renderer::d2

#endif /* _DORMOUSEENGINE_RENDERER_D2_LAYOUT_HPP_ */
