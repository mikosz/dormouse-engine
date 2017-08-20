#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_MODULE "graphics"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/system/platform.hpp"

#include "dormouse-engine/system/App.hpp"

#ifdef DE_COMPILER_VISUAL_CXX
#	pragma comment(linker, "/subsystem:windows")
#endif

namespace /* anonymous */
{

boost::unit_test::test_suite* initUnitTest(int, char**) {
	return nullptr;
}

} // anonymous namespace

DE_BEGIN_MAIN();
	return boost::unit_test::unit_test_main(&initUnitTest, __argc, __argv);
DE_END_MAIN();
