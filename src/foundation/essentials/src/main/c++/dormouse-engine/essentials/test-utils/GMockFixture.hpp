#ifndef DORMOUSEENGINE_ESSENTIALS_TEST_UTILS_GMOCKFIXTURE_HPP_
#define DORMOUSEENGINE_ESSENTIALS_TEST_UTILS_GMOCKFIXTURE_HPP_

#include <iosfwd>

#include <boost/filesystem/path.hpp>
#include <boost/test/unit_test.hpp>

#include <gmock/gmock.h>

namespace dormouse_engine {
namespace essentials {
namespace test_utils {

class GMockFixture {

	class BoostTestAdapter : public testing::EmptyTestEventListener {

		void OnTestStart(const testing::TestInfo&) {
		}

		void OnTestEnd(const testing::TestInfo&) {
		}

		void OnTestPartResult(const testing::TestPartResult& testPartResult) {
			if (testPartResult.failed()) {
				std::ostringstream err;
				err << "Mock test failed (file: "
					<< (testPartResult.file_name() ? testPartResult.file_name() : "")
					<< ", line: " << testPartResult.line_number()
					<< "): " << testPartResult.summary();
				BOOST_ERROR(err.str());
			}
		}

	};

public:

	GMockFixture() {
		testing::GTEST_FLAG(throw_on_failure) = false;
		testing::InitGoogleMock(&boost::unit_test::framework::master_test_suite().argc,
			boost::unit_test::framework::master_test_suite().argv);
		testing::TestEventListeners& listeners = testing::UnitTest::GetInstance()->listeners();
		delete listeners.Release(listeners.default_result_printer());
		listeners.Append(new BoostTestAdapter);
	}

};

} // namespace test_utils
} // namespace essentials
} // namespace dormouse_engine

// Wokaround for GMocks error that caused a stack overflow when mocking functions that accepted
// boost::filesystem::path parameters
namespace boost {
namespace filesystem {

inline void PrintTo(const boost::filesystem::path& path, std::ostream* os) {
	*os << path;
}

} // namespace filesystem
} // namespace boost

#endif /* DORMOUSEENGINE_ESSENTIALS_TEST_UTILS_GMOCKFIXTURE_HPP_ */
