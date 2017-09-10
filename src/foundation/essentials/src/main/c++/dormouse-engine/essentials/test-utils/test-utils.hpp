#ifndef DORMOUSEENGINE_ESSENTIALS_TEST_UTILS_HPP_
#define DORMOUSEENGINE_ESSENTIALS_TEST_UTILS_HPP_

#include <string>
#include <sstream>
#include <fstream>

#include <boost/filesystem.hpp>

#include "dormouse-engine/system/tmp-file.hpp"
#include "../memory.hpp"

namespace dormouse_engine::essentials::test_utils {

inline void writeToFile(const boost::filesystem::path& path, const std::string& s) {
	std::ofstream ofs(path.string().c_str());
	BOOST_ASSERT(ofs.good());
	ofs << s;
	ofs.close();
}

inline std::string readFile(const boost::filesystem::path& path) {
	std::ifstream ifs(path.string().c_str());
	BOOST_ASSERT(ifs.good());
	std::ostringstream oss;
	oss << ifs.rdbuf();
	return oss.str();
}

inline essentials::ByteVector readBinaryFile(const boost::filesystem::path& path) {
	const auto size = boost::filesystem::file_size(path);
	auto result =  essentials::ByteVector(size);
	std::ifstream ifs(path.string().c_str(), std::ios::binary);
	BOOST_ASSERT(ifs.good());
	ifs.read(reinterpret_cast<char*>(result.data()), result.size());
	BOOST_ASSERT(ifs.good());
	return result;
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

} // namespace dormouse_engine::essentials::test_utils

#endif /* DORMOUSEENGINE_ESSENTIALS_TEST_UTILS_HPP_ */
