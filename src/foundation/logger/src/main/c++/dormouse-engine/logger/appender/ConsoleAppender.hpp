#ifndef DORMOUSEENGINE_LOGGER_APPENDER_CONSOLEAPPENDER_HPP_
#define DORMOUSEENGINE_LOGGER_APPENDER_CONSOLEAPPENDER_HPP_

#include "Appender.hpp"

#include <string>
#include <iostream>

#include "dormouse-engine/essentials/policy/creation/New.hpp"

#include "dormouse-engine/logger/configuration/LoggerConfiguration.hpp"
#include "dormouse-engine/logger/layout/LayoutFactory.hpp"

namespace dormouse_engine::logger::appender {

class ConsoleAppender : public Appender {
public:

	static const std::string CLASS_NAME;

	ConsoleAppender(Level level, layout::LayoutSharedPtr layout) :
		Appender(level, layout)
	{
	}

	ConsoleAppender(
		const Id& id,
		const logger::configuration::LoggerConfiguration& configuration,
		layout::LayoutFactory* layoutFactory
		) :
		Appender(id, configuration, layoutFactory)
	{
	}

protected:

	void doAppend(const std::string& message) override {
		std::clog << message;
		std::clog.flush();
	}

private:

	ConsoleAppender() {
	}

	friend class essentials::Initialiser<
		Appender,
		const Id&,
		const logger::configuration::LoggerConfiguration&,
		layout::LayoutFactory*
		>;

};

} // namespace dormouse_engine::logger::appender

#endif /* DORMOUSEENGINE_LOGGER_APPENDER_CONSOLEAPPENDER_HPP_ */
