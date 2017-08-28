#ifndef _DORMOUSEENGINE_TESTER_MAIN_HPP_
#define _DORMOUSEENGINE_TESTER_MAIN_HPP_

#define BOOST_TEST_NO_MAIN

#ifdef DE_TEST_MODULE
#	define BOOST_TEST_MODULE DE_TEST_MODULE
#else
#	error "DE_TEST_MODULE not defined""
#endif /* DE_TEST_MODULE */

#include <boost/test/included/unit_test.hpp>

#include "dormouse-engine/wm/App.hpp"

namespace /* anonymous */
{

boost::unit_test::test_suite* initUnitTest(int, char**) {
	return nullptr;
}

} // anonymous namespace

DE_APP_MAIN()
	return boost::unit_test::unit_test_main(&initUnitTest, __argc, __argv);
}

#endif /* _DORMOUSEENGINE_TESTER_MAIN_HPP_ */
