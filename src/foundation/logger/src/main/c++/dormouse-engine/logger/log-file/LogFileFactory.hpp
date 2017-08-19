#ifndef DORMOUSEENGINE_LOGGER_LOGFILE_LOGFILEFACTORY_HPP_
#define DORMOUSEENGINE_LOGGER_LOGFILE_LOGFILEFACTORY_HPP_

#include <functional>

#include <boost/filesystem/path.hpp>

#include "dormouse-engine/essentials/policy/creation/Functor.hpp"
#include "dormouse-engine/factory.hpp"
#include "LogFile.hpp"

namespace dormouse_engine {
namespace logger {
namespace log_file {

class LogFileFactory {
public:

	void registerLogFile(const std::string& id, const boost::filesystem::path& path, bool overwrite);

	LogFileSharedPtr create(const std::string& id);

private:

	using FunctorType = std::function<LogFileUniquePtr()>;

	using Factory = Factory<
		std::string,
		factory::CreatorRegistry<
			std::string,
			policy::creation::Functor<FunctorType>,
			factory::error_policy::ExceptionThrowing
			>,
		factory::storage::Permanent
		>;

	Factory factory_;

};

DE_SMART_POINTER_DEFINITONS(LogFileFactory);

} // namespace log_file
} // namespace logger
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_LOGGER_LOGFILE_LOGFILEFACTORY_HPP_ */
