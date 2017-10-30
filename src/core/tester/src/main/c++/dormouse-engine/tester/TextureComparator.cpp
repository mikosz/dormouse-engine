#include "TextureComparator.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::tester;

namespace /* anonymous */ {



} // anonymous namespace

TextureComparator::TextureComparator(graphics::Device& graphicsDevice)
{
}

void TextureComparator::compare(
	graphics::Device& graphicsDevice,
	renderer::control::ResourceView reference,
	renderer::control::ResourceView actual
	) const
{
	auto& commandList = graphicsDevice.getImmediateCommandList();
	
}
