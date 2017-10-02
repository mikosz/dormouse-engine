#include "Object.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::reflection;

void detail::declareTag() {
	auto builder = ponder::Enum::declare<Tag>("dormouse_engine::reflection::Tag");

	for (auto tag : allTagValues()) {
		builder.value(toString(tag), tag);
	}
}

void detail::declareReflectiveObject() {
	ponder::Class::declare<ReflectiveObject>("dormouse_engine::reflection::ReflectiveObject");
}
