#ifndef DORMOUSEENGINE_LOGGER_APPENDER_FILEAPPENDER_HPP_
#define DORMOUSEENGINE_LOGGER_APPENDER_FILEAPPENDER_HPP_

#include <fstream>
#include <string>

#include "Appender.hpp"

#include "dormouse-engine/essentials/policy/creation/New.hpp"

#include "dormouse-engine/essentials/pointee.hpp"

#include "dormouse-engine/logger/log-file/LogFile.hpp"

namespace dormouse_engine::logger::appender {

class FileAppender : public Appender {
public:

	static const std::string CLASS_NAME;

	FileAppender(Level level, layout::LayoutSharedPtr layout, log_file::LogFileSharedPtr logFile) :
		Appender(level, layout),
		logFile_(logFile)
	{
	}

	FileAppender(
		const Id& id,
		const logger::configuration::LoggerConfiguration& configuration,
		layout::LayoutFactory* layoutFactoryPtr
		) :
		Appender(id, configuration, layoutFactoryPtr)
	{
		layout::LayoutFactory& layoutFactory = essentials::pointee(layoutFactoryPtr);
		doInitialise(id, configuration, &layoutFactory);
	}

protected:

	void doAppend(const std::string& message) override {
		return logFile_->write(message);
	}

	void doInitialise(
		const Id& id,
		const logger::configuration::LoggerConfiguration& configuration,
		layout::LayoutFactory* layoutFactory
		) override;

private:

	FileAppender() {
	}

	log_file::LogFileSharedPtr logFile_;

	friend class essentials::Initialiser<
		Appender,
		const Id&,
		const logger::configuration::LoggerConfiguration&,
		layout::LayoutFactory*
		>;

};

} // namespace dormouse_engine::logger::appender

#endif /* DORMOUSEENGINE_LOGGER_APPENDER_FILEAPPENDER_HPP_ */
