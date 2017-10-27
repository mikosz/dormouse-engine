#include "TextureComparator.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::tester;

namespace /* anonymous */ {



} // anonymous namespace

TextureComparator::TextureComparator(
	graphics::Device& graphicsDevice,
	renderer::control::ResourceView reference,
	renderer::control::ResourceView actual
	) :
	reference_(std::move(reference)),
	actual_(std::move(actual))
{
}
