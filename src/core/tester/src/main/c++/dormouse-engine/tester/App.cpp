#include "App.hpp"

#include <iostream>
#include <memory>

#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_MODULE "dormouse_engine::tester"
#include <boost/test/included/unit_test.hpp>

#include "dormouse-engine/system/platform.hpp"
#include "dormouse-engine/logger/Logger.hpp"
#include "dormouse-engine/logger/LoggerStringbuf.hpp"
#include "dormouse-engine/logger/appender/DebugWindowAppender.hpp"
#include "dormouse-engine/logger/layout/EmptyLayout.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::tester;

namespace /* anonymous */ {

dormouse_engine::wm::Window::Configuration windowConfiguration() {
	using namespace dormouse_engine;

	auto result = wm::Window::Configuration();

	result.className = "DormouseEngineTesterWindow";
	result.title = "dormouse_engine::tester";
	result.fullscreen = false;
	result.width = 800u;
	result.height = 600u;

	return result;
}

void redirectCErrToLogger() {
#if defined(DE_COMPILER_VISUAL_CXX)
	auto boostTestLogger = std::make_shared<logger::Logger>(logger::Level::INFO);
	auto boostTestLayout = std::make_shared<logger::layout::EmptyLayout>();
	auto boostTestAppender = std::make_shared<logger::appender::DebugWindowAppender>(
		logger::Level::INFO, std::move(boostTestLayout));

	boostTestLogger->addAppender(std::move(boostTestAppender));

	auto loggerStringbuf = logger::LoggerStringbuf<char>(std::move(boostTestLogger));
	std::cout.rdbuf(&loggerStringbuf);
	std::cerr.rdbuf(&loggerStringbuf);
	std::clog.rdbuf(&loggerStringbuf);

	// TODO: this probably won't work
	auto wLoggerStringbuf = logger::LoggerStringbuf<wchar_t>(std::move(boostTestLogger));
	std::wcout.rdbuf(&wLoggerStringbuf);
	std::wcerr.rdbuf(&wLoggerStringbuf);
	std::wclog.rdbuf(&wLoggerStringbuf);
#endif /* DE_COMPILER_VISUAL_CXX */
}

boost::unit_test::test_suite* initUnitTest(int, char**) {
	return nullptr;
}

} // anonymous namespace

App::App() :
	engineApp_(*wm::GlobalMainArguments::instance(), windowConfiguration())
{
}

void App::update() {
	engineApp_.update();
}

DE_APP_MAIN()
	redirectCErrToLogger();

	return boost::unit_test::unit_test_main(&initUnitTest, __argc, __argv);
}
