#ifndef _DORMOUSEENGINE_RENDERER_D2_LAYOUT_HPP_
#define _DORMOUSEENGINE_RENDERER_D2_LAYOUT_HPP_

#include <utility>

#pragma warning(push, 3)
#	include <ponder/pondertype.hpp>
#pragma warning(pop)

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

	const math::Transform toNDC() const noexcept;

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

namespace detail { void declareLayout(); }

} // namespace dormouse_engine::renderer::d2

PONDER_AUTO_TYPE(
	dormouse_engine::renderer::d2::Layout,
	&dormouse_engine::renderer::d2::detail::declareLayout
	);

#endif /* _DORMOUSEENGINE_RENDERER_D2_LAYOUT_HPP_ */
