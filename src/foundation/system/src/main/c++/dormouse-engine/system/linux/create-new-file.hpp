#ifndef DORMOUSEENGINE_SYSTEM_LINUX_CREATE_NEW_FILE_HPP_
#define DORMOUSEENGINE_SYSTEM_LINUX_CREATE_NEW_FILE_HPP_

#include <boost/filesystem/path.hpp>

namespace dormouse_engine {
namespace system {
namespace linux {

bool createNewFile(const boost::filesystem::path& path);

} // namespace linux
} // namespace system
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_SYSTEM_LINUX_CREATE_NEW_FILE_HPP_ */
