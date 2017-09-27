#include "Interface.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::reflection;

bool Interface::hasProperty(essentials::StringId name) const {
	return clazz_->hasFunction(name.string()) || clazz_->hasProperty(name.string());
}
