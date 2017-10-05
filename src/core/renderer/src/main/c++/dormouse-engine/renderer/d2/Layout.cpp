#include "Layout.hpp"

#pragma warning(push, 3)
#	include <ponder/classbuilder.hpp>
#pragma warning(pop)

#include "dormouse-engine/reflection/Object.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer::d2;

const math::Transform Layout::toNDC() const noexcept {
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

void detail::declareLayout() {
	ponder::Class::declare<Layout>("dormouse_engine::renderer::d2::Layout")
		.property("toNDC", &Layout::toNDC)
		;
}
