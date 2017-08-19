#ifndef DORMOUSEENGINE_LOGGER_MACROS_HPP_
#define DORMOUSEENGINE_LOGGER_MACROS_HPP_

#include "dormouse-engine/system/function-name.hpp"

#include "Category.hpp"
#include "Context.hpp"
#include "Level.hpp"
#include "GlobalLoggerFactory.hpp"

#define DE_LOGGER_CATEGORY(NAME) \
	namespace /* anonymous */ { \
	const dormouse_engine::logger::Category& loggerCategory(dormouse_engine::logger::FakeParam) { \
		static dormouse_engine::logger::Category category = (NAME); \
		return category; \
		} \
	} // anonymous namespace

// TODO: __pragma should be windows only
#define DE_LOGGER_LOCAL_CATEGORY(NAME) \
	__pragma(warning(suppress: 4456)); \
	auto loggerCategory = [](dormouse_engine::logger::FakeParam) { return (NAME); }

#define DE_LOGGER_CONTEXT dormouse_engine::logger::Context(loggerCategory(dormouse_engine::logger::FakeParam()), __FILE__, __LINE__, DE_FUNCTION_NAME)

#define DE_LOGGER dormouse_engine::logger::GlobalLoggerFactory::instance()->create(loggerCategory(dormouse_engine::logger::FakeParam()))

#define DE_LOG(LEVEL) \
	if ((LEVEL) < (DE_LOGGER)->getLevel()) { \
	} else (DE_LOGGER)->log((LEVEL), DE_LOGGER_CONTEXT)

#define DE_LOG_TRACE DE_LOG(dormouse_engine::logger::Level::TRACE)
#define DE_LOG_DEBUG DE_LOG(dormouse_engine::logger::Level::DEBUG)
#define DE_LOG_INFO DE_LOG(dormouse_engine::logger::Level::INFO)
#define DE_LOG_WARNING DE_LOG(dormouse_engine::logger::Level::WARNING)
#define DE_LOG_ERROR DE_LOG(dormouse_engine::logger::Level::ERROR)
#define DE_LOG_CRITICAL DE_LOG(dormouse_engine::logger::Level::CRITICAL)

#endif /* DORMOUSEENGINE_LOGGER_MACROS_HPP_ */
