#include "configuration.pch.hpp"

#include "ParseError.hpp"

#include <sstream>

using namespace dormouse_engine::configuration::parsers;

std::string ParseError::constructMessage(const boost::filesystem::path& thePath, size_t theLineNo) {
	std::ostringstream oss;
	oss << "Error while reading properties file \""
			<< (thePath.empty() ? "-" : thePath.string().c_str()) << "\" in line " << theLineNo;
	return oss.str();
}
