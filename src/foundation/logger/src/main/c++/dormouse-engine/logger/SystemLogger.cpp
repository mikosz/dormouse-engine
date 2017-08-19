#include "SystemLogger.hpp"

#include "dormouse-engine/system/platform.hpp"

#include "layout/EmptyLayout.hpp"
#include "appender/ConsoleAppender.hpp"
#include "appender/DebugWindowAppender.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;

SystemLogger::SystemLogger() :
	Logger(Level::INFO)
{
	layout::LayoutSharedPtr layout(new layout::EmptyLayout());
	addAppender(std::make_shared<appender::ConsoleAppender>(Level::TRACE, layout));

#if defined(DE_COMPILER_VISUAL_CXX)
	addAppender(std::make_shared<appender::DebugWindowAppender>(Level::TRACE, layout));
#endif /* DE_COMPILER_VISUAL_CXX */
}
