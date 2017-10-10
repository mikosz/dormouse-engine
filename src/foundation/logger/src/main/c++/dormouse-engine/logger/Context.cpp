#include "logger.pch.hpp"

#include "Context.hpp"

using namespace dormouse_engine::logger;

const Context Context::EMPTY;

const Context& Context::empty() {
	return EMPTY;
}
