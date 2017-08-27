#include "configuration.pch.hpp"

#include "SelectorHas.hpp"

#include "dormouse-engine/configuration/hierarchical/HierarchicalConfiguration.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::configuration;
using namespace dormouse_engine::configuration::hierarchical;
using namespace dormouse_engine::configuration::hierarchical::node;

bool SelectorHas::matches(const HierarchicalConfiguration& node) const {
	return node.count(nodePath_) != 0;
}

std::string SelectorHas::string() const {
	return "HAS: " + nodePath_.string();
}
