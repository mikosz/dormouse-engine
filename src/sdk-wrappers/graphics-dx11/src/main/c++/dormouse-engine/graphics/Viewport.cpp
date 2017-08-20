#include "Viewport.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

Viewport::Viewport(const Configuration& configuration) {
	viewport_.Width = configuration.width;
	viewport_.Height = configuration.height;
	viewport_.MinDepth = configuration.minDepth;
	viewport_.MaxDepth = configuration.maxDepth;
	viewport_.TopLeftX = configuration.topLeftX;
	viewport_.TopLeftY = configuration.topLeftY;
}
