#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/configuration/hierarchical/node/Path.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::configuration;
using namespace dormouse_engine::configuration::hierarchical;
using namespace dormouse_engine::configuration::hierarchical::node;

BOOST_AUTO_TEST_SUITE(PathTestSuite);

BOOST_AUTO_TEST_CASE(EmptyPathIsEmpty) {
	BOOST_CHECK(Path().empty());
}

BOOST_AUTO_TEST_CASE(EmptyStringYieldsEmptyPath) {
	Path nodePath("");
	BOOST_CHECK(nodePath.empty());
}

BOOST_AUTO_TEST_CASE(OperatorDivideAppendsPath) {
	auto nodePath = Path() / "a" / "b" / "c";
	BOOST_CHECK_EQUAL(nodePath.root(), "a");
	BOOST_CHECK_EQUAL(nodePath.childPath().root(), "b");
	BOOST_CHECK_EQUAL(nodePath.childPath().childPath().root(), "c");
}

BOOST_AUTO_TEST_CASE(AppendedPathIsParsedAndDivided) {
	auto nodePath = Path("a/b") / "c/d";
	BOOST_CHECK_EQUAL(nodePath.root(), "a");
	BOOST_CHECK_EQUAL(nodePath.childPath().root(), "b");
	BOOST_CHECK_EQUAL(nodePath.childPath().childPath().root(), "c");
	BOOST_CHECK_EQUAL(nodePath.childPath().childPath().childPath().root(), "d");
}

BOOST_AUTO_TEST_CASE(SamePathsYieldEqual) {
	// deilberately added empty nodes, which should be removed automatically
	BOOST_CHECK_EQUAL(Path("/a/b/c"), Path("a/b//c/"));
}

BOOST_AUTO_TEST_CASE(DifferentPathsYieldUnequal) {
	// deilberately added empty nodes, which should be removed automatically
	BOOST_CHECK_NE(Path("a/b/c"), Path("a/d/c"));
}

BOOST_AUTO_TEST_CASE(AppendingSelectorWithoutPathAddsToLastNode) {
	auto original = Path("a/b/c")["selector"];
	auto appended = Path().is("text");

	auto expected = Path("a/b/c")["selector"].is("text");

	BOOST_CHECK_EQUAL(original / appended, expected);
}

BOOST_AUTO_TEST_CASE(PrependingSelectorWithoutPathAddsEmptyNodePath) {
	auto original = Path("a/b/c")["selector"];
	auto prepended = Path().is("text");

	auto expected = Path().is("text") / Path("a/b/c")["selector"];

	BOOST_CHECK_EQUAL(prepended / original, expected);
	BOOST_CHECK_EQUAL((prepended / original).root(), Path().is("text"));
}

BOOST_AUTO_TEST_CASE(DifferentValueSelectorsYieldUnequal) {
	auto lhs = (Path() / "name")["selector-1"];
	auto rhs = (Path() / "name")["selector-2"];

	BOOST_CHECK_NE(lhs, rhs);
}

BOOST_AUTO_TEST_CASE(DifferentTypeSelectorsYieldUnequal) {
	auto lhs = (Path() / "name")["selector"];
	auto rhs = (Path() / "name").is("selector");

	BOOST_CHECK_NE(lhs, rhs);
}

BOOST_AUTO_TEST_CASE(SameSelectorsYieldEqual) {
	auto lhs = (Path() / "name")[Path("subNode").is("selector")];
	auto rhs = (Path() / "name")[Path("subNode").is("selector")];

	BOOST_CHECK_EQUAL(lhs, rhs);
}

BOOST_AUTO_TEST_SUITE_END(/* PathTestSuite */);
