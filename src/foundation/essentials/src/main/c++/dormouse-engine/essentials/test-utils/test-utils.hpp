#ifndef DORMOUSEENGINE_ESSENTIALS_TEST_UTILS_HPP_
#define DORMOUSEENGINE_ESSENTIALS_TEST_UTILS_HPP_

#include <string>
#include <sstream>
#include <fstream>

#include <boost/filesystem.hpp>

#include "dormouse-engine/system/tmp-file.hpp"

namespace dormouse_engine {
namespace essentials {
namespace test_utils {

inline void writeToFile(const boost::filesystem::path& path, const std::string& s) {
	std::ofstream ofs(path.string().c_str());
	ofs << s;
	ofs.close();
}

inline std::string readFile(const boost::filesystem::path& path) {
	std::ifstream ifs(path.string().c_str());
	std::ostringstream oss;
	oss << ifs.rdbuf();
	return oss.str();
}

class ResourcesDirFixture {
public:

	ResourcesDirFixture() :
		resourcesDir_(system::createTmpDir(".test-resources-")) {
	}

	~ResourcesDirFixture() {
		boost::filesystem::remove_all(resourcesDir_);
	}

	const boost::filesystem::path& resourcesDir() const {
		return resourcesDir_;
	}

private:

	boost::filesystem::path resourcesDir_;

};

} // namespace test_utils
} // namespace essentials
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_ESSENTIALS_TEST_UTILS_HPP_ */
