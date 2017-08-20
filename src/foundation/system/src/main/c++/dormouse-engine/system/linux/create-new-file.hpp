#ifndef DORMOUSEENGINE_SYSTEM_LINUX_CREATE_NEW_FILE_HPP_
#define DORMOUSEENGINE_SYSTEM_LINUX_CREATE_NEW_FILE_HPP_

#include <boost/filesystem/path.hpp>

namespace dormouse_engine::system::linux {

bool createNewFile(const boost::filesystem::path& path);

} // namespace dormouse_engine::system::linux

#endif /* DORMOUSEENGINE_SYSTEM_LINUX_CREATE_NEW_FILE_HPP_ */
