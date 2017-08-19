#include "DetailedLayout.hpp"

#include <sstream>

using namespace dormouse_engine;
using namespace dormouse_engine::logger;
using namespace dormouse_engine::logger::layout;

const std::string DetailedLayout::CLASS_NAME("dormouse_engine::logger::layout::DetailedLayout");

std::string DetailedLayout::format(Level level, const Context& context, const std::string& message) {
	std::ostringstream oss;
	clock_.formatNow(&oss);
	oss << " @ " << context.file << ':' << context.line << " [" << level << "] - "
		<< context.category << '\n' << message << "\n\n";

	return oss.str();
}

void DetailedLayout::doInitialise(
	const Id& layoutId,
	const logger::configuration::LoggerConfiguration& configuration
	) {
	auto configurationNode = configuration.layoutConfiguration(layoutId);
	try {
		clock_.precision() = configurationNode->getAs<Clock::Precision>("time-precision");
	} catch (const dormouse_engine::configuration::BadValueType&) {
		throw logger::configuration::LoggerConfigurationError(
			"Bad time-precision value for layout " + layoutId + ": \"" + configurationNode->get("time-precision")->text() + "\".");
	} catch (const dormouse_engine::configuration::MissingRequiredValue&) {
		throw logger::configuration::LoggerConfigurationError("Missing required configuration value \"time-precision\" for layout " + layoutId);
	}
}
