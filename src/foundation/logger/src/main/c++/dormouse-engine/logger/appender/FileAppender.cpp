#include "FileAppender.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;
using namespace dormouse_engine::logger::appender;

const std::string FileAppender::CLASS_NAME("dormouse_engine::logger::appender::FileAppender");

void FileAppender::doInitialise(
	const Id& id,
	const logger::configuration::LoggerConfiguration& configuration,
	layout::LayoutFactory* layoutFactoryPtr
	) {
	layout::LayoutFactory& layoutFactory = essentials::pointee(layoutFactoryPtr);
	Appender::doInitialise(id, configuration, &layoutFactory);

	logFile_ = configuration.logFile(id);
}
