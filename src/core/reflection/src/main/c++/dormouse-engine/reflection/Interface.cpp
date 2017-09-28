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

ponder::Value Interface::getPropertyValue_(
	const Object& object, essentials::StringId name, [[maybe_unused]] Tag withTag) const
{
	const auto& property = clazz_->property(name.string());

	assert(withTag == Tag::NONE || property.hasTag(withTag));

	return property.get(object);
}
