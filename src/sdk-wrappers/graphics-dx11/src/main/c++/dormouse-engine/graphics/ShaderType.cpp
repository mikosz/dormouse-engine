#include "graphics.pch.hpp"

#include "ShaderType.hpp"

#pragma warning(push, 3)
#	include <ponder/enum.hpp>
#	include <ponder/enumbuilder.hpp>
#pragma warning(pop)

using namespace dormouse_engine::graphics;

void detail::declareShaderType() {
	auto builder = ponder::Enum::declare<ShaderType>("dormouse_engine::graphics::ShaderType");

	for (auto tag : allShaderTypeValues()) {
		builder.value(toString(tag), tag);
	}
}
