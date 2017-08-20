#ifndef DORMOUSEENGINE_CONFIGURATION_PARSERS_PROPERTIESPARSER_HPP_
#define DORMOUSEENGINE_CONFIGURATION_PARSERS_PROPERTIESPARSER_HPP_

#include "FlatParser.hpp"

namespace dormouse_engine::configuration::parsers {

class PropertiesParser : public FlatParser {
public:

	using FlatParser::parse;

	void parse(std::istream& is, KeyValueCallback keyValueCallback) const;

};

} // namespace dormouse_engine::configuration::parsers

#endif /* DORMOUSEENGINE_CONFIGURATION_PARSERS_PROPERTIESPARSER_HPP_ */
