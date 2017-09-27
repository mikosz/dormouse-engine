#include "Object.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::reflection;

Interface reflection::reflectObject(const ReflectiveObject& object) {
	const auto& clazz = ponder::classByName(object.name_.string());
	return essentials::make_observer(&clazz);
}
