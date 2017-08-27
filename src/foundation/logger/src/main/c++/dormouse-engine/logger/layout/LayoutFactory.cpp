#include "logger.pch.hpp"

#include "LayoutFactory.hpp"

#include "dormouse-engine/essentials/pointee.hpp"

#include "dormouse-engine/system/platform.hpp"

#include "BasicLayout.hpp"
#include "DetailedLayout.hpp"
#include "EmptyLayout.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;
using namespace dormouse_engine::logger::layout;

namespace {

void registerBuiltins(LayoutFactory* factoryPtr) {
	LayoutFactory& factory = essentials::pointee(factoryPtr);

	factory.registerType<BasicLayout>(BasicLayout::CLASS_NAME);
	factory.registerType<DetailedLayout>(DetailedLayout::CLASS_NAME);
	factory.registerType<EmptyLayout>(EmptyLayout::CLASS_NAME);
}

} // anonymous namespace

LayoutFactory::LayoutFactory(logger::configuration::ConstLoggerConfigurationSharedPtr loggerConfiguration) :
	loggerConfiguration_(loggerConfiguration)
{
	registerBuiltins(this);
}

void LayoutFactory::reloadConfiguration(logger::configuration::ConstLoggerConfigurationSharedPtr loggerConfiguration) {
	instanceStorage_.clear();
	loggerConfiguration_ = loggerConfiguration;
}

LayoutSharedPtr LayoutFactory::create(const Layout::Id& layoutId) {
	return instanceStorage_.get(layoutId, [this, &layoutId]() {
			auto initialiser = typeFactory_.create(loggerConfiguration_->layoutTypeId(layoutId));
			return initialiser->initialise(layoutId, *loggerConfiguration_);
		});
}
