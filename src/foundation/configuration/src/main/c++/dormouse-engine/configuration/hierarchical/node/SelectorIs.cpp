#include "configuration.pch.hpp"

#include "SelectorIs.hpp"

#include "dormouse-engine/configuration/hierarchical/HierarchicalConfiguration.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::configuration;
using namespace dormouse_engine::configuration::hierarchical;
using namespace dormouse_engine::configuration::hierarchical::node;

bool SelectorIs::matches(const HierarchicalConfiguration& node) const {
	return node.text() == nodeText_;
}

std::string SelectorIs::string() const {
	return "IS: " + nodeText_;
}
