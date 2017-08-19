#include "LoggerConfiguration.hpp"

#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

using namespace dormouse_engine;
using namespace dormouse_engine::logger;
using namespace dormouse_engine::logger::configuration;

using dormouse_engine::configuration::hierarchical::node::Path;
using dormouse_engine::configuration::hierarchical::ConstHierarchicalConfigurationSharedPtr;
using dormouse_engine::configuration::hierarchical::HierarchicalConfiguration;

namespace /* anonymous */ {

const ConstHierarchicalConfigurationSharedPtr EMPTY_NODE;

ConstHierarchicalConfigurationSharedPtr getNode(
	ConstHierarchicalConfigurationSharedPtr configuration,
	const Path& basePath,
	const std::string& id,
	const Path& childPath
	) {
	return configuration->getOptional(basePath[Path("id").is(id)] / childPath, EMPTY_NODE);
}

HierarchicalConfiguration::Nodes getNodes(
	ConstHierarchicalConfigurationSharedPtr configuration,
	const Path& basePath,
	const std::string& id,
	const Path& childPath
	) {
	HierarchicalConfiguration::Nodes nodes;
	configuration->getAll(basePath[Path("id").is(id)] / childPath, &nodes);
	return nodes;
}

ConstHierarchicalConfigurationSharedPtr getDerivedNode(
	ConstHierarchicalConfigurationSharedPtr configuration,
	const Path& basePath,
	const std::string& id,
	const Path& childPath
	) {
	std::vector<std::string> elements;
	boost::split(elements, id, boost::is_any_of("."));

	while (!elements.empty()) {
		auto nodeId = boost::join(elements, ".");
		auto node = getNode(configuration, basePath, nodeId, childPath);

		if (node) {
			return node;
		}

		elements.pop_back();
	}

	return EMPTY_NODE;
}

HierarchicalConfiguration::Nodes getDerivedNodes(
	ConstHierarchicalConfigurationSharedPtr configuration,
	const Path& basePath,
	const std::string& id,
	const Path& childPath,
	const Path& aggregateWithParentPath
	) {
	std::vector<std::string> elements;
	boost::split(elements, id, boost::is_any_of("."));

	HierarchicalConfiguration::Nodes result;

	while (!elements.empty()) {
		auto nodeId = boost::join(elements, ".");
		auto nodes = getNodes(configuration, basePath, nodeId, childPath);

		if (!nodes.empty()) {
			std::copy(nodes.begin(), nodes.end(), std::back_inserter(result));
			
			auto aggregateWithParentPathNode = getNode(configuration, basePath, nodeId, aggregateWithParentPath);
			if (aggregateWithParentPathNode && !boost::lexical_cast<bool>(aggregateWithParentPathNode->text())) {
				break;
			}
		}

		elements.pop_back();
	}

	return result;
}

ConstHierarchicalConfigurationSharedPtr getRootLoggerNode(
	ConstHierarchicalConfigurationSharedPtr configuration,
	const Path& childPath
	) {
	return configuration->getOptional("root-logger" / childPath, EMPTY_NODE);
}

HierarchicalConfiguration::Nodes getRootLoggerNodes(
	ConstHierarchicalConfigurationSharedPtr configuration,
	const Path& childPath
	) {
	HierarchicalConfiguration::Nodes result;
	configuration->getAll("root-logger" / childPath, &result);
	return result;
}

ConstHierarchicalConfigurationSharedPtr getLoggerNode(
	ConstHierarchicalConfigurationSharedPtr configuration,
	const Path& basePath,
	const std::string& id,
	const Path& childPath
	) {
	auto node = getDerivedNode(configuration, basePath, id, childPath);
	if (!node) {
		node = getRootLoggerNode(configuration, childPath);
	}

	return node;
}

HierarchicalConfiguration::Nodes getLoggerNodes(
	ConstHierarchicalConfigurationSharedPtr configuration,
	const Path& basePath,
	const std::string& id,
	const Path& childPath,
	const Path& aggregateWithParentPath
	) {
	auto nodes = getDerivedNodes(configuration, basePath, id, childPath, aggregateWithParentPath);
	
	if (nodes.empty()) {
		nodes = getRootLoggerNodes(configuration, childPath);
	}

	return nodes;
}

void registerLogFiles(log_file::LogFileFactory& logFileFactory, ConstHierarchicalConfigurationSharedPtr configuration) {
	HierarchicalConfiguration::Nodes nodes;
	configuration->getAll("log-files/log-file", &nodes);

	for (auto node : nodes) {
		try {
			auto id = node->getAs<std::string>("id");
			auto path = node->getAs<boost::filesystem::path>("path");
			auto overwrite = node->getAs<bool>("overwrite");

			logFileFactory.registerLogFile(id, path, overwrite);
		} catch (const dormouse_engine::configuration::MissingRequiredValue& e) {
			throw LoggerConfigurationError(e.what());
		}
	}
}

} // anonymous namespace

LoggerConfiguration::LoggerConfiguration(Configuration config, log_file::LogFileFactorySharedPtr logFileFactory) :
	configuration_(config),
	logFileFactory_(logFileFactory)
{
	registerLogFiles(*logFileFactory_, configuration_);
}

Level LoggerConfiguration::loggerLevel(const LoggerId& loggerId) const {
	auto node = getLoggerNode(configuration_, "loggers/logger", loggerId, "level");
	if (!node) {
		throw LoggerConfigurationError("level option not specified for logger \"" + loggerId + '"');
	}

	return boost::lexical_cast<Level>(node->text());
}

Level LoggerConfiguration::appenderLevel(const AppenderId& appenderId) const {
	auto node = getDerivedNode(configuration_, "appenders/appender", appenderId, "level");
	if (!node) {
		return Level::TRACE;
	}

	return boost::lexical_cast<Level>(node->text());
}

LoggerConfiguration::AppenderIds LoggerConfiguration::appenderIds(const LoggerId& loggerId) const {
	auto nodes =
		getLoggerNodes(configuration_, "loggers/logger", loggerId, "appenders/appender", "aggregate-with-parent");

	AppenderIds result;

	for (auto node : nodes) {
		result.push_back(node->text());
	}

	return result;
}

LoggerConfiguration::LayoutId LoggerConfiguration::layoutId(const AppenderId& appenderId) const {
	auto node = getDerivedNode(configuration_, "appenders/appender", appenderId, "layout");
	if (!node) {
		throw LoggerConfigurationError("layout option not specified for appender \"" + appenderId + '"');
	}

	return node->text();
}

LoggerConfiguration::AppenderTypeId LoggerConfiguration::appenderTypeId(const AppenderId& appenderId) const {
	auto node = getDerivedNode(configuration_, "appenders/appender", appenderId, "type");
	if (!node) {
		throw LoggerConfigurationError("type option not specified for appender \"" + appenderId + '"');
	}

	return node->text();
}

log_file::LogFileSharedPtr LoggerConfiguration::logFile(const AppenderId& appenderId) const {
	try {
		auto node = getDerivedNode(configuration_, "appenders/appender", appenderId, "log-file");
		if (!node) {
			throw LoggerConfigurationError("log-file option not specified for appender \"" + appenderId + '"');
		}

		return logFileFactory_->create(node->text());
	} catch (const factory::error_policy::NoSuchType<std::string>&) {
		throw LoggerConfigurationError("Unknown log file: " + appenderId);
	}
}

LoggerConfiguration::LayoutId LoggerConfiguration::layoutTypeId(const LayoutId& layoutId) const {
	auto node = getDerivedNode(configuration_, "layouts/layout", layoutId, "type");
	if (!node) {
		throw LoggerConfigurationError("type option not specified for layout \"" + layoutId + '"');
	}

	return node->text();
}

LoggerConfiguration::Configuration LoggerConfiguration::layoutConfiguration(const LayoutId& layoutId) const {
	auto node = getDerivedNode(configuration_, "layouts/layout", layoutId, "configuration");
	if (!node) {
		throw LoggerConfigurationError("configuration option not specified for layout \"" + layoutId + '"');
	}

	return node;
}
