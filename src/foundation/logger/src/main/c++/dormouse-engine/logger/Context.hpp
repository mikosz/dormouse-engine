#ifndef DORMOUSEENGINE_LOGGER_CONTEXT_HPP_
#define DORMOUSEENGINE_LOGGER_CONTEXT_HPP_

#include <string>

#include "Category.hpp"

namespace dormouse_engine::logger {

struct Context {

	static const Context& empty();

	Context() :
		line(0)
	{
	}

	Context(const Category& category, const std::string& file, size_t line, const std::string& function) :
		category(category),
		file(file),
		line(line),
		function(function)
	{
	}

	Category category;

	std::string file;

	size_t line;

	std::string function;

private:

	static const Context EMPTY;

};

} // namespace dormouse_engine::logger

#endif /* DORMOUSEENGINE_LOGGER_CONTEXT_HPP_ */
