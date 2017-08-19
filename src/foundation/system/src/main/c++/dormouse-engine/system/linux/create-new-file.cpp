#include "../platform.hpp"

#if defined(DE_PLATFORM_LINUX)

#include "create-new-file.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../SystemError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::system;
using namespace dormouse_engine::system::linux;

bool dormouse_engine::system::linux::createNewFile(const boost::filesystem::path& path) {
	if (open(path.string().c_str(), O_CREAT | O_EXCL, S_IRUSR | S_IWUSR) == -1) {
		if (errno == EEXIST) {
			return false;
		} else {
			throw SystemError("Failed to create file " + path.string(),
				std::error_code(errno, std::system_category()));
		}
	} else {
		return true;
	}
}

#endif /* DE_PLATFORM_LINUX */
