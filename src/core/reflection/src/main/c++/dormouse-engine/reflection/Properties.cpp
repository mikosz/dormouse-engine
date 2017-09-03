#include "Properties.hpp"

#include <cassert>
#include <boost/lexical_cast.hpp>

using namespace dormouse_engine;
using namespace dormouse_engine::reflection;
using namespace dormouse_engine::reflection;

namespace /* anonymous */ {

template <class Value>
const Value& childProperty(essentials::StringId id, const std::unordered_map<essentials::StringId, Value>& children) {
	const auto it = children.find(id);
	if (it == children.end()) {
		throw PropertyNotBound(id);
	}
	return it->second;
}

} // anonymous namespace

void Properties::bindResource(
	command::DrawCommand& cmd,
	const PropertyId& id,
	void* object,
	graphics::ShaderType stage,
	size_t slot
	) const
{
	const auto& idHead = id.head();
	const auto idTail = id.tail();

	assert(idHead.arraySize == 0u);

	if (!idTail.empty()) {
		const auto child = childProperty(idHead.name, children_);
		
		???
		child.iface_(object)
			
			->bindResource(cmd, idTail, stage, slot);
	} else {
		childProperty(idHead.name, resourceProperties_).bind(cmd, stage, slot);
	}
}
