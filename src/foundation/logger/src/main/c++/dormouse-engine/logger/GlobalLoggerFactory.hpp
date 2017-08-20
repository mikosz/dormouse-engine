#ifndef DORMOUSEENGINE_LOGGER_GLOBALLOGGERFACTORY_HPP_
#define DORMOUSEENGINE_LOGGER_GLOBALLOGGERFACTORY_HPP_

#include <boost/filesystem/path.hpp>

#include "dormouse-engine/essentials/Singleton.hpp"

#include "LoggerFactory.hpp"

namespace dormouse_engine::logger {

class GlobalLoggerFactory :
	public essentials::Singleton<
		GlobalLoggerFactory,
		essentials::policy::creation::New<GlobalLoggerFactory>
		>,
	public LoggerFactory
{
public:

	static const boost::filesystem::path& defaultConfigurationPath();

	GlobalLoggerFactory();

	using LoggerFactory::reloadConfiguration;

	void reloadConfiguration(const boost::filesystem::path& path = defaultConfigurationPath());

private:

	static const boost::filesystem::path DEFAULT_CONFIGURATION_PATH;

};

} // namespace dormouse_engine::logger

#endif /* DORMOUSEENGINE_LOGGER_GLOBALLOGGERFACTORY_HPP_ */
