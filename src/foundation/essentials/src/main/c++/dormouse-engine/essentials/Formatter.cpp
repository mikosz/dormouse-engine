#include "Formatter.hpp"

#include <sstream>

#include <boost/lexical_cast.hpp>

using namespace dormouse_engine::essentials;

std::string FormatterError::constructMessage(const std::string& message, const std::string& formattedString,
		std::string::size_type pos) {
	std::ostringstream oss;
	oss << "Got error: " << message << R"( while formatting )" << formattedString << R"(" at pos: )" << pos;
	return oss.str();
}

void Formatter::format(FormatList& result, const std::string& s) const {
	enum {
		READY,
		ESCAPE,
		FORMAT_MARKER,
		STRING,
		OPTS,
		OPTS_READ
	} state, prevState;

	std::ostringstream oss;
	std::string::size_type end = s.length();
	state = READY;
	prevState = state;
	for (std::string::size_type pos = 0; pos != end; ++pos) {
		if (s[pos] == formatterChars_.escape) {
			switch (state) {
			case READY:
			case STRING:
			case OPTS:
				prevState = state;
				state = ESCAPE;
				break;
			case ESCAPE:
				oss << formatterChars_.escape;
				state = prevState;
				break;
			default:
				throw FormatterError("Unexpected escape character", s, pos);
			}
		} else if (s[pos] == formatterChars_.formatMarker) {
			switch (state) {
			case READY:
				state = FORMAT_MARKER;
				break;
			case ESCAPE:
				oss << formatterChars_.formatMarker;
				state = prevState;
				break;
			case STRING:
				result.emplace_back('\0', oss.str());
				oss.str("");
				state = FORMAT_MARKER;
				break;
			default:
				throw FormatterError("Unexpected format marker", s, pos);
			}
		} else if (s[pos] == formatterChars_.formatOptionsStart) {
			switch (state) {
			case READY:
			case STRING:
				oss << formatterChars_.formatOptionsStart;
				break;
			case ESCAPE:
				oss << formatterChars_.formatOptionsStart;
				state = prevState;
				break;
			case FORMAT_MARKER:
				state = OPTS;
				break;
			default:
				throw FormatterError("Unexpected opts start marker", s, pos);
			}
		} else if (s[pos] == formatterChars_.formatOptionsEnd) {
			switch (state) {
			case READY:
			case STRING:
				oss << formatterChars_.formatOptionsEnd;
				break;
			case ESCAPE:
				oss << formatterChars_.formatOptionsEnd;
				state = prevState;
				break;
			case OPTS:
				state = OPTS_READ;
				break;
			default:
				throw FormatterError("Unexpected opts end marker", s, pos);
			}
		} else {
			switch (state) {
			case READY:
				state = STRING;
				oss << s[pos];
				break;
			case STRING:
			case OPTS:
				oss << s[pos];
				break;
			case FORMAT_MARKER:
			case OPTS_READ:
				result.emplace_back(s[pos], oss.str());
				oss.str("");
				state = READY;
				break;
			default:
				throw FormatterError("Unexpected character", s, pos);
			}
		}
	}

	switch (state) {
	case READY:
		break;
	case STRING:
		result.emplace_back('\0', oss.str());
		break;
	default:
		throw FormatterError("Unexpected end of string", s, s.length());
	}
}
