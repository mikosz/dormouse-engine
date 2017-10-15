#ifndef _DORMOUSEENGINE_TESTER_MAIN_HPP_
#define _DORMOUSEENGINE_TESTER_MAIN_HPP_

#define BOOST_TEST_NO_MAIN

#ifdef DE_TEST_MODULE
#	define BOOST_TEST_MODULE DE_TEST_MODULE
#else
#	error "DE_TEST_MODULE not defined""
#endif /* DE_TEST_MODULE */

#include <iostream>

#include "dormouse-engine/system/platform.hpp"
#include "dormouse-engine/logger/Logger.hpp"
#include "dormouse-engine/logger/LoggerStringbuf.hpp"
#include "dormouse-engine/logger/appender/DebugWindowAppender.hpp"
#include "dormouse-engine/logger/layout/EmptyLayout.hpp"
#include "dormouse-engine/engine/app/App.hpp"

#include <boost/test/included/unit_test.hpp>

namespace /* anonymous */
{

boost::unit_test::test_suite* initUnitTest(int, char**) {
	return nullptr;
}

// TODO: temp
inline dormouse_engine::wm::Window::Configuration windowConfiguration() {
	using namespace dormouse_engine;
	
	auto result = wm::Window::Configuration();

	result.className = "RenderingFixtureWindow";
	result.title = "Rendering fixture window";
	result.fullscreen = false;
	result.width = 800u;
	result.height = 600u;

	return result;
}

} // anonymous namespace

DE_APP_MAIN()
	using namespace dormouse_engine;

	//auto app = engine::app::App(*wm::GlobalMainArguments::instance(), windowConfiguration());

	//app.run();

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

	return boost::unit_test::unit_test_main(&initUnitTest, __argc, __argv);
}

#endif /* _DORMOUSEENGINE_TESTER_MAIN_HPP_ */
