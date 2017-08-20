#ifndef DORMOUSEENGINE_CONFIGURATION_CONFIGURATIONUPDATER_HPP_
#define DORMOUSEENGINE_CONFIGURATION_CONFIGURATIONUPDATER_HPP_

#include <cstdlib>
#include <functional>

#include <boost/unordered_map.hpp>

namespace dormouse_engine::configuration {

template <class ConfigurationType>
class ConfigurationUpdater {
public:

	typedef ConfigurationType Configuration;

	typedef std::function<void (const Configuration&)> VerifyFunc;

	typedef std::function<void (const Configuration&)> UpdateFunc;

	typedef void* Listener;

	~ConfigurationUpdater() {
		if (!listeners_.empty()) {
			// This is a serious logic error which would cause a segmentation fault.
			// Not throwing an exception as this is a destructor.
			// XXX: Log error here!
			std::abort();
		}
	}

	void registerListener(Listener listener, VerifyFunc verify, UpdateFunc update) {
		listeners_.insert(std::make_pair(listener, ListenerEntry(verify, update)));
	}

	void unregisterListener(Listener listener) {
		listeners_.erase(listener);
	}

	void update(const Configuration& configuration);

private:

	struct ListenerEntry {

		VerifyFunc verify;

		UpdateFunc update;

		ListenerEntry(VerifyFunc verify, UpdateFunc update) :
			verify(verify),
			update(update) {
		}

	};

	typedef boost::unordered_map<Listener, ListenerEntry> Listeners;

	Listeners listeners_;

};

template<class ConfigurationType>
void ConfigurationUpdater<ConfigurationType>::update(const Configuration& configuration) {
	typename Listeners::iterator it, end = listeners_.end();
	for (it = listeners_.begin(); it != end; ++it) {
		it->second.verify(configuration);
	}
	for (it = listeners_.begin(); it != end; ++it) {
		it->second.update(configuration);
	}
}

} // namespace dormouse_engine::configuration

#endif /* DORMOUSEENGINE_CONFIGURATION_CONFIGURATIONUPDATER_HPP_ */
