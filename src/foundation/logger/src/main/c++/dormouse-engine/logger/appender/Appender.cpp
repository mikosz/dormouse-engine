#include "logger.pch.hpp"

#include "Appender.hpp"

#include "dormouse-engine/essentials/pointee.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;
using namespace dormouse_engine::logger::appender;

Appender::Appender(
	const Id& id,
	const logger::configuration::LoggerConfiguration& configuration,
	layout::LayoutFactory* layoutFactoryPtr
	)
{
	doInitialise(id, configuration, layoutFactoryPtr);
}

void Appender::doInitialise(
	const Id& id,
	const logger::configuration::LoggerConfiguration& configuration,
	layout::LayoutFactory* layoutFactoryPtr
	) {
	layout::LayoutFactory& layoutFactory = essentials::pointee(layoutFactoryPtr);

	level_ = configuration.appenderLevel(id);
	layout_ = layoutFactory.create(configuration.layoutId(id));
}

void Appender::append(Level level, const Context& context, const std::string& message) {
	if (level_ <= level) {
		doAppend(layout_->format(level, context, message));
	}
}
