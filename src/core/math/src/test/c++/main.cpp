#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "dormouse-engine/system/platform.hpp"

#ifdef CT_COMPILER_VISUAL_CXX
#	pragma comment(linker, "/subsystem:console")
#endif
