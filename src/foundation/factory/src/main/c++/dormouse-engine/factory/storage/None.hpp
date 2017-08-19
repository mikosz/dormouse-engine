#ifndef DORMOUSEENGINE_FACTORY_STORAGE_NONE_HPP_
#define DORMOUSEENGINE_FACTORY_STORAGE_NONE_HPP_

#include <functional>
#include <memory>

#include <boost/call_traits.hpp>

namespace dormouse_engine {
namespace factory {
namespace storage {

template <class IdentifierType, class InstanceType>
class None {
public:

	typedef IdentifierType Identifier;

	typedef typename boost::call_traits<Identifier>::param_type IdentifierParam;

	typedef InstanceType Instance;

	typedef Instance InstanceParam;

	typedef Instance Permanent;

	using Creator = std::function<InstanceType()>;

	Instance get(const IdentifierParam /*identifier*/, Creator creator) {
		return creator();
	}

	bool isStored(const IdentifierParam) const {
		return false;
	}

	Permanent store(const IdentifierParam, InstanceParam&& instance) {
		return std::move(instance);
	}

	void erase(const IdentifierParam) {
	}

};

} // namespace storage
} // namespace factory
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_FACTORY_STORAGE_NONE_HPP_ */
