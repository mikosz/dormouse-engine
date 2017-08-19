#ifndef DORMOUSEENGINE_CONFIGURATION_PARSERS_PROPERTIESPARSER_HPP_
#define DORMOUSEENGINE_CONFIGURATION_PARSERS_PROPERTIESPARSER_HPP_

#include "FlatParser.hpp"

namespace dormouse_engine {
namespace configuration {
namespace parsers {

class PropertiesParser : public FlatParser {
public:

	using FlatParser::parse;

	void parse(std::istream& is, KeyValueCallback keyValueCallback) const;

};

}  // namespace parsers
}  // namespace configuration
}  // namespace dormouse_engine

#endif /* DORMOUSEENGINE_CONFIGURATION_PARSERS_PROPERTIESPARSER_HPP_ */
