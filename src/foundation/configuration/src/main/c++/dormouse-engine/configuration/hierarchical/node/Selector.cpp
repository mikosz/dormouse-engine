#include "configuration.pch.hpp"

#include "Selector.hpp"

#include <iostream>

using namespace dormouse_engine;
using namespace dormouse_engine::configuration;
using namespace dormouse_engine::configuration::hierarchical;
using namespace dormouse_engine::configuration::hierarchical::node;

std::ostream& dormouse_engine::configuration::hierarchical::node::operator<<(
	std::ostream& os, const Selector& selector) {
	return os << selector.string();
}
