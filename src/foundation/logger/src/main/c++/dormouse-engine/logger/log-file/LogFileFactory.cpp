#include "LogFileFactory.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;
using namespace dormouse_engine::logger::log_file;

namespace /* anonymous */ {

LogFileUniquePtr create(const boost::filesystem::path& path, bool overwrite) {
	return LogFileUniquePtr(new LogFile(path, overwrite));
}

} // anonymous namespace

void LogFileFactory::registerLogFile(
	const std::string& id,
	const boost::filesystem::path& path,
	bool overwrite
	) {
	factory_.registerCreator(
		id,
		essentials::policy::creation::Functor<FunctorType>([path, overwrite]() { return ::create(path, overwrite); })
		);
}

LogFileSharedPtr LogFileFactory::create(const std::string& id) {
	return factory_.create(id);
}
