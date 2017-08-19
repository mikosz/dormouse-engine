#ifndef DORMOUSEENGINE_UTILS_FORMATTER_HPP_
#define DORMOUSEENGINE_UTILS_FORMATTER_HPP_

#include <vector>
#include <string>

#include "dormouse-engine/exceptions/RuntimeError.hpp"

namespace dormouse_engine {
namespace essentials {

class FormatterError : public exceptions::RuntimeError {
public:

	FormatterError(const std::string& theMessage, const std::string& theFormattedString,
			std::string::size_type thePos) :
			exceptions::RuntimeError(constructMessage(theMessage, theFormattedString, thePos)),
			formattedString_(theFormattedString),
			pos_(thePos) {
	}

	~FormatterError() throw() {
	}

	const std::string& name() const noexcept override {
		static const std::string& NAME = "FormatterError";
		return NAME;
	}

	const std::string& formattedString() const {
		return formattedString_;
	}

	std::string::size_type pos() const {
		return pos_;
	}

private:

	std::string formattedString_;

	std::string::size_type pos_;

	static std::string constructMessage(const std::string& message, const std::string& formattedString,
			std::string::size_type pos);

};

class Formatter {
public:

	struct FormatterChars {

		char formatMarker;

		char formatOptionsStart;

		char formatOptionsEnd;

		char escape;

		FormatterChars(char formatMarker, char formatOptionsStart, char formatOptionsEnd, char escape) :
			formatMarker(formatMarker),
			formatOptionsStart(formatOptionsStart),
			formatOptionsEnd(formatOptionsEnd),
			escape(escape) {
		}

	};

	struct FormatNode {

		char function;

		std::string opts;

		FormatNode(char function, const std::string& opts) :
			function(function),
			opts(opts) {
		}

	};

	typedef std::vector<FormatNode> FormatList;

	Formatter(const FormatterChars& formatterChars) :
		formatterChars_(formatterChars) {
	}

	void format(FormatList& result, const std::string& s) const;

private:

	FormatterChars formatterChars_;

};

} // namespace essentials
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_UTILS_FORMATTER_HPP_ */
