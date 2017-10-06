#include "graphics.pch.hpp"

#include "ShaderDataType.hpp"

#pragma warning(push, 3)
#	include <ponder/classbuilder.hpp>
#	include <ponder/enumbuilder.hpp>
#pragma warning(pop)

using namespace dormouse_engine::graphics;

void detail::declareShaderDataTypeClass() {
	auto builder =
		ponder::Enum::declare<ShaderDataType::Class>("dormouse_engine::graphics::ShaderDataType::Class");

	for (auto tag : ShaderDataType::allClassValues()) {
		builder.value(toString(tag), tag);
	}
}

void detail::declareShaderDataTypeScalarType() {
	auto builder =
		ponder::Enum::declare<ShaderDataType::ScalarType>("dormouse_engine::graphics::ShaderDataType::ScalarType");

	for (auto tag : ShaderDataType::allScalarTypeValues()) {
		builder.value(toString(tag), tag);
	}
}

void detail::declareShaderDataType() {
	ponder::Class::declare<ShaderDataType>("dormouse_engine::graphics::ShaderDataType")
		.property("klass", &ShaderDataType::klass)
		.property("scalarType", &ShaderDataType::scalarType)
		.property("columns", &ShaderDataType::columns)
		.property("rows", &ShaderDataType::rows)
		;
}
