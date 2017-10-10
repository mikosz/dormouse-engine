#include "configuration.pch.hpp"

#include "XMLParser.hpp"

#include <boost/property_tree/xml_parser.hpp>

#include "dormouse-engine/essentials/pointee.hpp"

using namespace dormouse_engine::configuration::parsers;

void XMLParser::doParse(std::istream& is, boost::property_tree::ptree* treePtr) const {
	boost::property_tree::ptree& tree = essentials::pointee(treePtr);
	boost::property_tree::xml_parser::read_xml(is, tree);
}
