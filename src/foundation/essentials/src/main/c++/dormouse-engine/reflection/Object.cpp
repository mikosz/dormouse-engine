#include "Object.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::reflection;

void detail::declareClassTag() {
	auto builder = ponder::Enum::declare<ClassTag>("dormouse_engine::reflection::ClassTag");

	for (auto tag : allClassTagValues()) {
		builder.value(toString(tag), tag);
	}
}

void detail::declarePropertyTag() {
	auto builder = ponder::Enum::declare<PropertyTag>("dormouse_engine::reflection::PropertyTag");

	for (auto tag : allPropertyTagValues()) {
		builder.value(toString(tag), tag);
	}
}

void detail::declareFunctionTag() {
	auto builder = ponder::Enum::declare<FunctionTag>("dormouse_engine::reflection::FunctionTag");

	for (auto tag : allFunctionTagValues()) {
		builder.value(toString(tag), tag);
	}
}
