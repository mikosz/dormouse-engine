#include "PropertyTreeParser.hpp"

#include <boost/bind.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace dormouse_engine::configuration::parsers;

namespace {

void addTreeNodes(
		const std::string& name,
		boost::property_tree::ptree tree,
		PropertyTreeParser::NewChildCallback newChildCallback,
		PropertyTreeParser::ChildrenEndCallback childrenEndCallback
		) {
	std::string trimmedData = boost::trim_copy(tree.data());
	newChildCallback(name, trimmedData);

	for (const auto& node : tree) {
		addTreeNodes(node.first, node.second, newChildCallback, childrenEndCallback);
	}

	childrenEndCallback();
}

} // anonymous namespace

void PropertyTreeParser::parse(std::istream& is, NewChildCallback newChildCallback,
		ChildrenEndCallback childrenEndCallback) const {
	boost::property_tree::ptree tree;

	doParse(is, &tree);

	for (const auto& node : tree) {
		addTreeNodes(node.first, node.second, newChildCallback, childrenEndCallback);
	}
}
