#ifndef DORMOUSEENGINE_SINGLETON_SINGLETON_HPP_
#define DORMOUSEENGINE_SINGLETON_SINGLETON_HPP_

#include <cstdlib>
#include <memory>

#include <boost/noncopyable.hpp>

#include "policy/creation/New.hpp"

namespace dormouse_engine::essentials {

template <
	class InstanceType,
	class CreatorType = essentials::policy::creation::New<InstanceType>
	>
class Singleton : boost::noncopyable {
public:

	using Instance = InstanceType;

	using InstancePtr = std::shared_ptr<InstanceType>;

	using Creator = CreatorType;

	static InstancePtr instance() {
		if (!instance_) {
			Creator creator;
			instance_.reset(creator.create().release());
			std::atexit(&Singleton::destroy);
		}

		return instance_;
	}

	static void setInstance(std::unique_ptr<Instance>&& instance) {
		if (!instance_) {
			std::atexit(&Singleton::destroy);
		}
		instance_.reset(instance.release());
	}

private:

	static InstancePtr instance_;

	static void destroy() {
		if (instance_) {
			instance_.reset();
		}
	}

};

template <class InstanceType, class Creator>
typename Singleton<InstanceType, Creator>::InstancePtr Singleton<InstanceType, Creator>::instance_;

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_SINGLETON_SINGLETON_HPP_ */
