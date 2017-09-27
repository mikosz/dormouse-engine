#include "Interface.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::reflection;

void detail::declareTag() {
	auto builder = ponder::Enum::declare<Tag>("dormouse_engine::reflection::Tag");

	for (auto tag : allTagValues()) {
		builder.value(toString(tag), tag);
	}
}

essentials::StringId Interface::name() const {
	return clazz_->name();
}

bool Interface::hasProperty(essentials::StringId name, Tag withTag) const {
	return
		(clazz_->hasFunction(name.string()) || clazz_->hasProperty(name.string())) &&
		(withTag == Tag::NONE || clazz_->hasTag(withTag));
}
