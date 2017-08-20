#ifndef DORMOUSEENGINE_CONFIGURATION_CONFIGURABLE_HPP_
#define DORMOUSEENGINE_CONFIGURATION_CONFIGURABLE_HPP_

#include <functional>

#include "ConfigurationUpdater.hpp"

namespace dormouse_engine::configuration {

template <class ConfigurationType>
class Configurable {
protected:

	typedef ConfigurationType Configuration;

	Configurable(ConfigurationUpdater<Configuration>& configurationUpdater) :
		configurationUpdater_(configurationUpdater) {
		configurationUpdater_.registerListener(
				this,
				std::bind(&Configurable::verify, this, std::placeholders::_1),
				std::bind(&Configurable::update, this, std::placeholders::_1));
	}

	virtual ~Configurable() {
		configurationUpdater_.unregisterListener(this);
	}

	virtual void verify(const Configuration& config) const = 0;

	virtual void update(const Configuration& config) = 0;

private:

	ConfigurationUpdater<Configuration>& configurationUpdater_;

};

} // namespace dormouse_engine::configuration

#endif /* DORMOUSEENGINE_CONFIGURATION_CONFIGURABLE_HPP_ */
