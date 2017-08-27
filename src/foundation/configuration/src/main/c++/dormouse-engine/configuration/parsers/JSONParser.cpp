#include "configuration.pch.hpp"

#include "JSONParser.hpp"

#include "dormouse-engine/system/platform.hpp"

#if defined(DE_COMPILER_VISUAL_CXX)
#	pragma warning(push)
#	pragma warning(disable: 4715) // boost json_parser emits "not all control paths return a value"
#endif /* DE_COMPILER_VISUAL_CXX */

#include <boost/property_tree/json_parser.hpp>

#if defined(DE_COMPILER_VISUAL_CXX)
#	pragma warning(pop)
#endif /* DE_COMPILER_VISUAL_CXX */

#include "dormouse-engine/essentials/pointee.hpp"

using namespace dormouse_engine::configuration::parsers;

void JSONParser::doParse(std::istream& is, boost::property_tree::ptree* treePtr) const {
	boost::property_tree::ptree& tree = essentials::pointee(treePtr);
	boost::property_tree::json_parser::read_json(is, tree);
}
