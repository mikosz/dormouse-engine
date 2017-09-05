#include "Properties.hpp"

#include <cassert>
#include <boost/lexical_cast.hpp>

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;

const Property& Properties::get(essentials::StringId id) const {
	const auto it = properties_.find(id);
	if (it == properties_.end()) {
		throw PropertyNotBound(id);
	}
	return it->second;
}
