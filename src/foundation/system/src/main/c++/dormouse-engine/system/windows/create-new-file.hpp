#ifndef DORMOUSEENGINE_SYSTEM_WINDOWS_CREATE_NEW_FILE_HPP_
#define DORMOUSEENGINE_SYSTEM_WINDOWS_CREATE_NEW_FILE_HPP_

#include <boost/filesystem/path.hpp>

namespace dormouse_engine::system::windows {

bool createNewFile(const boost::filesystem::path& path);

} // namespace dormouse_engine::system::windows

#endif /* DORMOUSEENGINE_SYSTEM_WINDOWS_CREATE_NEW_FILE_HPP_ */
