#ifndef DORMOUSEENGINE_LOGGER_LOGFILE_LOGFILE_HPP_
#define DORMOUSEENGINE_LOGGER_LOGFILE_LOGFILE_HPP_

#include <fstream>

#include <boost/filesystem/path.hpp>

#include "dormouse-engine/system/SystemError.hpp"

#include "dormouse-engine/essentials/smart-pointer-definitions.hpp"

namespace dormouse_engine {
namespace logger {
namespace log_file {

class LogFile {
public:

	explicit LogFile(const boost::filesystem::path& path, bool overwrite) :
		stream_(
				path.string().c_str(),
				overwrite ? std::ios::trunc : std::ios::app
				)
	{
		if (!stream_) {
			throw system::SystemError(
				"Failed to open log file \"" + path.string() + "\" for writing",
				std::error_code(errno, std::system_category()) // XXX: why does iostream_category fail here?
				);
		}
	}

	virtual ~LogFile() {
	}

	virtual void write(const std::string& message) {
		stream_ << message;
		stream_.flush();
	}

private:

	std::ofstream stream_;

};

DE_SMART_POINTER_DEFINITONS(LogFile);

} // namespace log_file
} // namespace logger
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_LOGGER_LOGFILE_LOGFILE_HPP_ */
