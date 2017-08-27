#include "logger.pch.hpp"

#include "GlobalLoggerFactory.hpp"

#include <boost/filesystem.hpp>

#include "dormouse-engine/configuration/parsers/XMLParser.hpp"
#include "dormouse-engine/configuration/readers/HierarchicalConfigurationReader.hpp"

#include "dormouse-engine/logger/log-file/GlobalLogFileFactory.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::logger;

namespace /* anonymous */ {

logger::configuration::ConstLoggerConfigurationSharedPtr globalConfiguration(const boost::filesystem::path& path) {
	if (boost::filesystem::exists(path)) {
		auto configuration = dormouse_engine::configuration::hierarchical::HierarchicalConfiguration::create();
		dormouse_engine::configuration::parsers::XMLParser parser;
		dormouse_engine::configuration::readers::HierarchicalConfigurationReader reader;
		reader.read(parser, path, configuration.get());
		return logger::configuration::LoggerConfigurationSharedPtr(
			new logger::configuration::LoggerConfiguration(configuration, log_file::GlobalLogFileFactory::instance())
			);
	} else {
		return logger::configuration::LoggerConfigurationSharedPtr();
	}
}

} // anonymous namespace

const boost::filesystem::path GlobalLoggerFactory::DEFAULT_CONFIGURATION_PATH("dormouse-engine-logger.cfg.xml");

const boost::filesystem::path& GlobalLoggerFactory::defaultConfigurationPath() {
	return DEFAULT_CONFIGURATION_PATH;
}

GlobalLoggerFactory::GlobalLoggerFactory() :
	LoggerFactory(globalConfiguration(DEFAULT_CONFIGURATION_PATH))
{
}

void GlobalLoggerFactory::reloadConfiguration(const boost::filesystem::path& path) {
	reloadConfiguration(globalConfiguration(path));
}
