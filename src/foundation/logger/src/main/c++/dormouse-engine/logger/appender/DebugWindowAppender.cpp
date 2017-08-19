#include "dormouse-engine/system/platform.hpp"

#if defined(DE_COMPILER_VISUAL_CXX)

#include "DebugWindowAppender.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "dormouse-engine/system/platform.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;
using namespace dormouse_engine::logger::appender;

const std::string DebugWindowAppender::CLASS_NAME("dormouse_engine::logger::appender::DebugWindowAppender");

void DebugWindowAppender::doAppend(const std::string& message) {
	OutputDebugStringA(message.c_str());
}

#endif /* DE_COMPILER_VISUAL_CXX */
