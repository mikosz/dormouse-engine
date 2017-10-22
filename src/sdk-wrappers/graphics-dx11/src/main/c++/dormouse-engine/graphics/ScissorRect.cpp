#include "graphics.pch.hpp"

#include "ScissorRect.hpp"

using namespace dormouse_engine::graphics;

ScissorRect::ScissorRect(const Configuration& configuration) {
	enabled_ = true;
	scissorRect_.bottom = configuration.bottom;
	scissorRect_.left = configuration.left;
	scissorRect_.right = configuration.right;
	scissorRect_.top = configuration.top;
}
