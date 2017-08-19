#include "HierarchicalParser.hpp"

#include <fstream>

#include "ParseError.hpp"

using namespace dormouse_engine::configuration::parsers;

void HierarchicalParser::parse(const boost::filesystem::path& path, NewChildCallback newChildCallback,
		ChildrenEndCallback childrenEndCallback) const {
	std::ifstream ifs(path.string().c_str());

	try {
		parse(ifs, newChildCallback, childrenEndCallback);
		ifs.close();
	} catch (const ParseError& e) {
		throw ParseError(path, e.lineNo(), e);
	}
}
