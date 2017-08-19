#ifndef DORMOUSEENGINE_LOGGER_LOGGERCONFIGURATION_HPP_
#define DORMOUSEENGINE_LOGGER_LOGGERCONFIGURATION_HPP_

#include <memory>
#include <string>

#include "dormouse-engine/configuration/hierarchical/HierarchicalConfiguration.hpp"

#include "dormouse-engine/exceptions/RuntimeError.hpp"

#include "dormouse-engine/essentials/smart-pointer-definitions.hpp"

#include "dormouse-engine/logger/Level.hpp"
#include "dormouse-engine/logger/log-file/LogFile.hpp"
#include "dormouse-engine/logger/log-file/LogFileFactory.hpp"

namespace dormouse_engine {
namespace logger {
namespace configuration {

class LoggerConfigurationError : public exceptions::RuntimeError {
public:

	LoggerConfigurationError(const std::string& message) :
		exceptions::RuntimeError(message)
	{
	}

};

/**
 * <pre>
 *   <root-logger>
 *	 <level>INFO</level>
 *   </root-logger>
 *   <loggers>
 *	 <logger>
 *	   <id>logger-id</id>
 *	   <level>DEBUG</level>
 *	   <appender-ids>
 *		 <appender-id>console-appender</appender-id>
 *		 <appender-id>file-appender</appender-id>
 *	   </appender-ids>
 *	 </logger>
 *	 <logger>
 *	   <id>logger-id.child</id>
 *	   <level>INFO</level>
 *	 </logger>
 *   </loggers>
 *   <appenders>
 *	 <appender>
 *	   <id>console-appender</id>
 *	   <type>dormouse_engine::logger::appender::ConsoleAppender</type>
 *	   <layout-id>layout-id</layout-id>
 *	 </appender>
 *	 <appender>
 *	   <id>file-appender</id>
 *	   <type>dormouse_engine::logger::appender::FileAppender</type>
 *	   <configuration>
 *		 <log-file-id>file-id</log-file-id>
 *		 <layout-id>layout-id</layout-id>
 *	   </configuration>
 *	 </appender>
 *	 <layouts>
 *	   <layout>
 *		 <id>layout-id</id>
 *		 <type>dormouse_engine::logger::layout::EmptyLayout</type>
 *	   </layout>
 *	 </layouts>
 *	 <log-files>
 *	   <log-file>
 *		 <id>file-id</id>
 *		 <path>path/to/a/log/file.log</path>
 *		 <overwrite>false</overwrite>
 *	   </log-file>
 *	 </log-files>
 * </pre>
 */
class LoggerConfiguration {
public:

	typedef std::string LoggerId;

	typedef std::string AppenderId;

	typedef std::vector<AppenderId> AppenderIds;

	typedef std::string AppenderTypeId;

	typedef std::string LayoutId;

	typedef dormouse_engine::configuration::hierarchical::ConstHierarchicalConfigurationSharedPtr Configuration;

	LoggerConfiguration(Configuration config, log_file::LogFileFactorySharedPtr logFileFactory);

	virtual ~LoggerConfiguration() {
	}

	Level loggerLevel(const LoggerId& loggerId) const;

	Level appenderLevel(const AppenderId& appenderId) const;

	AppenderIds appenderIds(const LoggerId& loggerId) const;

	LayoutId layoutId(const AppenderId& appenderId) const;

	AppenderTypeId appenderTypeId(const AppenderId& appenderId) const;

	log_file::LogFileSharedPtr logFile(const AppenderId& appenderId) const;

	LayoutId layoutTypeId(const LayoutId& layoutId) const;

	Configuration layoutConfiguration(const LayoutId& layoutId) const;

private:

	Configuration configuration_;

	log_file::LogFileFactorySharedPtr logFileFactory_;

};

DE_SMART_POINTER_DEFINITONS(LoggerConfiguration);

}  // namespace configuration
}  // namespace logger
}  // namespace dormouse_engine

#endif /* DORMOUSEENGINE_LOGGER_LOGGERCONFIGURATION_HPP_ */
