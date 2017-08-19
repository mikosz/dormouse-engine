#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/essentials/Formatter.hpp"

using namespace dormouse_engine::essentials;

namespace {

BOOST_AUTO_TEST_SUITE(FormatterTestSuite);

BOOST_AUTO_TEST_CASE(FormatsEmptyString) {
	std::string s;
	Formatter f(Formatter::FormatterChars('$', '{', '}', '\\'));
	Formatter::FormatList result;
	f.format(result, s);
	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(FormatsNoSpecial) {
	std::string s = "No special characters here ({} is not special if no dollar sign before)";
	Formatter f(Formatter::FormatterChars('$', '{', '}', '\\'));
	Formatter::FormatList result;
	f.format(result, s);
	BOOST_REQUIRE(result.size() == 1);
	BOOST_CHECK_EQUAL(result.front().function, '\0');
	BOOST_CHECK_EQUAL(result.front().opts,  s);
}

BOOST_AUTO_TEST_CASE(FormatsAllEscaped) {
	std::string s = "No special characters here (neither \\$f nor \\${opts}f is not special if \\$ is preceded by \\\\)";
	std::string escaped = "No special characters here (neither $f nor ${opts}f is not special if $ is preceded by \\)";
	Formatter f(Formatter::FormatterChars('$', '{', '}', '\\'));
	Formatter::FormatList result;
	f.format(result, s);
	BOOST_REQUIRE(result.size() == 1);
	BOOST_CHECK_EQUAL(result.front().function, '\0');
	BOOST_CHECK_EQUAL(result.front().opts,  escaped);
}

BOOST_AUTO_TEST_CASE(FormatsOnlySpecial) {
	std::string s = "%1%[opts]2%3";
	Formatter f(Formatter::FormatterChars('%', '[', ']', '\\'));
	Formatter::FormatList result;
	f.format(result, s);

	BOOST_REQUIRE(result.size() == 3);

	Formatter::FormatList::iterator it = result.begin();

	BOOST_CHECK_EQUAL(it->function, '1');
	BOOST_CHECK_EQUAL(it->opts, "");
	++it;

	BOOST_CHECK_EQUAL(it->function, '2');
	BOOST_CHECK_EQUAL(it->opts, "opts");
	++it;

	BOOST_CHECK_EQUAL(it->function, '3');
	BOOST_CHECK_EQUAL(it->opts, "");
}

BOOST_AUTO_TEST_CASE(FormatsMixed) {
	std::string s = "This is a mixed %s with opts %(012\\(\\)3)d and an escaped \\% char";
	Formatter f(Formatter::FormatterChars('%', '(', ')', '\\'));
	Formatter::FormatList result;
	f.format(result, s);

	BOOST_REQUIRE(result.size() == 5);

	Formatter::FormatList::iterator it = result.begin();

	BOOST_CHECK_EQUAL(it->function, '\0');
	BOOST_CHECK_EQUAL(it->opts, "This is a mixed ");
	++it;

	BOOST_CHECK_EQUAL(it->function, 's');
	BOOST_CHECK_EQUAL(it->opts, "");
	++it;

	BOOST_CHECK_EQUAL(it->function, '\0');
	BOOST_CHECK_EQUAL(it->opts, " with opts ");
	++it;

	BOOST_CHECK_EQUAL(it->function, 'd');
	BOOST_CHECK_EQUAL(it->opts, "012()3");
	++it;

	BOOST_CHECK_EQUAL(it->function, '\0');
	BOOST_CHECK_EQUAL(it->opts, " and an escaped % char");
}

BOOST_AUTO_TEST_CASE(ThrowsOnMissingFunction) {
	Formatter f(Formatter::FormatterChars('$', '{', '}', '\\'));
	Formatter::FormatList result;

	std::string s = "$";
	BOOST_CHECK_THROW(f.format(result, s), FormatterError);

	s = "${}";
	BOOST_CHECK_THROW(f.format(result, s), FormatterError);

	s = "$$a";
	BOOST_CHECK_THROW(f.format(result, s), FormatterError);
}

BOOST_AUTO_TEST_CASE(ThrowsOnUnclosedOpts) {
	Formatter f(Formatter::FormatterChars('$', '{', '}', '\\'));
	Formatter::FormatList result;

	std::string s = "${opts$n";
	BOOST_CHECK_THROW(f.format(result, s), FormatterError);
}

BOOST_AUTO_TEST_CASE(ThrowsOnExtraEscape) {
	Formatter f(Formatter::FormatterChars('$', '{', '}', '!'));
	Formatter::FormatList result;

	std::string s = "Just a text with an extra ! char";
	BOOST_CHECK_THROW(f.format(result, s), FormatterError);

	s = "Text and format with escaped {: $!{";
	BOOST_CHECK_THROW(f.format(result, s), FormatterError);
}

BOOST_AUTO_TEST_SUITE_END();

} // anonymous namespace
