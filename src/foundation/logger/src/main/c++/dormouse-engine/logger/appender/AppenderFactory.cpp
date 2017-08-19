#include "AppenderFactory.hpp"

#include "ConsoleAppender.hpp"
#include "DebugWindowAppender.hpp"
#include "FileAppender.hpp"

#include "dormouse-engine/essentials/pointee.hpp"

#include "dormouse-engine/system/platform.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;
using namespace dormouse_engine::logger::appender;

namespace {

void registerBuiltins(AppenderFactory* factoryPtr) {
	AppenderFactory& factory = essentials::pointee(factoryPtr);

	factory.registerType<ConsoleAppender>(ConsoleAppender::CLASS_NAME);
	factory.registerType<FileAppender>(FileAppender::CLASS_NAME);
#if defined(DE_COMPILER_VISUAL_CXX)
	factory.registerType<DebugWindowAppender>(DebugWindowAppender::CLASS_NAME);
#endif /* DE_COMPILER_VISUAL_CXX */
}

} // anonymous namespace

AppenderFactory::AppenderFactory(logger::configuration::ConstLoggerConfigurationSharedPtr loggerConfiguration) :
	loggerConfiguration_(loggerConfiguration),
	layoutFactory_(loggerConfiguration)
{
	registerBuiltins(this);
}

void AppenderFactory::reloadConfiguration(logger::configuration::ConstLoggerConfigurationSharedPtr loggerConfiguration) {
	instanceStorage_.clear();
	loggerConfiguration_ = loggerConfiguration;
	layoutFactory_.reloadConfiguration(loggerConfiguration_);
}

AppenderSharedPtr AppenderFactory::create(const Appender::Id& appenderId) {
	return instanceStorage_.get(appenderId, [this, &appenderId]() {
			auto initialiser = typeFactory_.create(loggerConfiguration_->appenderTypeId(appenderId));
			return initialiser->initialise(appenderId, *loggerConfiguration_, &layoutFactory_);
		});
}
