#ifndef DORMOUSEENGINE_SYSTEM_TMP_FILE_HPP_
#define DORMOUSEENGINE_SYSTEM_TMP_FILE_HPP_

#include <string>

#include <boost/filesystem/path.hpp>

namespace dormouse_engine::system {

boost::filesystem::path createTmpFile(const std::string& prefix = "", const std::string& suffix = "");

boost::filesystem::path createTmpDir(const std::string& prefix = "", const std::string& suffix = "");

} // namespace dormouse_engine::system

#endif /* DORMOUSEENGINE_SYSTEM_TMP_FILE_HPP_ */
