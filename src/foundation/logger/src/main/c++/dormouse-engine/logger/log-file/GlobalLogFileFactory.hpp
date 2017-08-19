#ifndef DORMOUSEENGINE_LOGGER_LOGFILE_GLOBALLOGFILEFACTORY_HPP_
#define DORMOUSEENGINE_LOGGER_LOGFILE_GLOBALLOGFILEFACTORY_HPP_

#include <boost/thread/mutex.hpp>

#include "dormouse-engine/essentials/policy/creation/New.hpp"

#include "dormouse-engine/essentials/Singleton.hpp"

#include "LogFileFactory.hpp"

namespace dormouse_engine {
namespace logger {
namespace log_file {

class GlobalLogFileFactory :
	public essentials::Singleton<
		GlobalLogFileFactory,
		policy::creation::New<GlobalLogFileFactory>
		>,
	public LogFileFactory
{
};

} // namespace log_file
} // namespace logger
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_LOGGER_LOGFILE_GLOBALLOGFILEFACTORY_HPP_ */
