#include "PropertiesParser.hpp"

#include <string>
#include <fstream>

#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "ParseError.hpp"

using namespace dormouse_engine::configuration::parsers;

namespace {

const boost::regex EMPTY_LINE_REGEX("^\\s*(#.*)?$");
const boost::regex CATEGORY_REGEX("^\\s*\\[([^\\]]+)\\]\\s*$");
const boost::regex VALUE_REGEX("^([^=]+)=(.*)$");

} // anonymous namespace

void PropertiesParser::parse(std::istream& is, KeyValueCallback keyValueCallback) const {
	std::string line;
	size_t lineNo = 0;
	std::string categoryPrefix;

	while (std::getline(is, line)) {
		++lineNo;
		boost::smatch matches;
		if (boost::regex_match(line, EMPTY_LINE_REGEX)) {
		} else if (boost::regex_match(line, matches, CATEGORY_REGEX)) {
			categoryPrefix = boost::trim_copy(matches[1].str()) + ".";
		} else if (boost::regex_match(line, matches, VALUE_REGEX)) {
			std::string key = boost::trim_copy(matches[1].str());
			std::string value = boost::trim_copy(matches[2].str());
			keyValueCallback(categoryPrefix + key, value);
		} else {
			throw ParseError(lineNo);
		}
	}
}
