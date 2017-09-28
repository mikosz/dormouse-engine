#include "Interface.hpp"

#include <cassert>

#include "Object.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::reflection;

void detail::declareTag() {
	auto builder = ponder::Enum::declare<Tag>("dormouse_engine::reflection::Tag");

	for (auto tag : allTagValues()) {
		builder.value(toString(tag), tag);
	}
}

Interface::Interface(essentials::StringId className) :
	clazz_(essentials::make_observer(&ponder::classByName(className.string())))
{
}

essentials::StringId Interface::name() const {
	return clazz_->name();
}

bool Interface::hasProperty(essentials::StringId name, Tag withTag) const {
	if (clazz_->hasFunction(name.string())) {
		return withTag == Tag::NONE || clazz_->function(name.string()).hasTag(withTag);
	} else {
		return withTag == Tag::NONE || clazz_->property(name.string()).hasTag(withTag);
	}
}
