#ifndef DORMOUSEENGINE_SYSTEM_CREATE_NEW_FILE_HPP_
#define DORMOUSEENGINE_SYSTEM_CREATE_NEW_FILE_HPP_

#include "platform.hpp"

#if defined(DE_PLATFORM_WINDOWS)
#	include "windows/create-new-file.hpp"
using dormouse_engine::system::windows::createNewFile;
#elif defined(DE_PLATFORM_LINUX)
#	include "linux/create-new-file.hpp"
using dormouse_engine::system::linux::createNewFile;
#else
#	error "createNewFile not available for this platform"
#endif 

#endif /* DORMOUSEENGINE_SYSTEM_CREATE_NEW_FILE_HPP_ */
