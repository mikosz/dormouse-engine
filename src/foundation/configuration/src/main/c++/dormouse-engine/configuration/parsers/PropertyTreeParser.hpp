#ifndef DORMOUSEENGINE_CONFIGURATION_PARSERS_PROPERTYTREEPARSER_HPP_
#define DORMOUSEENGINE_CONFIGURATION_PARSERS_PROPERTYTREEPARSER_HPP_

#include "HierarchicalParser.hpp"

#include <boost/property_tree/ptree.hpp>

namespace dormouse_engine::configuration::parsers {

class PropertyTreeParser : public HierarchicalParser {
public:

	using HierarchicalParser::parse;

	void parse(std::istream& is, NewChildCallback newChildCallback, ChildrenEndCallback childrenEndCallback) const;

protected:

	virtual void doParse(std::istream& is, boost::property_tree::ptree* tree) const = 0;

};

} // namespace dormouse_engine::configuration::parsers

#endif /* DORMOUSEENGINE_CONFIGURATION_PARSERS_PROPERTYTREEPARSER_HPP_ */
