#ifndef DORMOUSEENGINE_LOGGER_LOGGERFACTORY_HPP_
#define DORMOUSEENGINE_LOGGER_LOGGERFACTORY_HPP_

#include <string>

#include "dormouse-engine/logger/configuration/LoggerConfiguration.hpp"
#include "dormouse-engine/factory/storage/Permanent.hpp"
#include "appender/ConsoleAppender.hpp"
#include "appender/AppenderFactory.hpp"
#include "layout/LayoutFactory.hpp"
#include "Logger.hpp"

namespace dormouse_engine::logger {

class LoggerFactory {
public:

	typedef std::string LoggerId;

	LoggerFactory(logger::configuration::ConstLoggerConfigurationSharedPtr loggerConfiguration);

	void reloadConfiguration(logger::configuration::ConstLoggerConfigurationSharedPtr loggerConfiguration);

	LoggerSharedPtr create(const LoggerId& loggerId);

	appender::AppenderFactory& appenderFactory() {
		return appenderFactory_;
	}

private:

	configuration::ConstLoggerConfigurationSharedPtr loggerConfiguration_;

	factory::storage::Permanent<LoggerId, std::unique_ptr<Logger>> storage_;

	appender::AppenderFactory appenderFactory_;

};

} // namespace dormouse_engine::logger

#endif /* DORMOUSEENGINE_LOGGER_LOGGERFACTORY_HPP_ */
