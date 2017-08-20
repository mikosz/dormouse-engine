#ifndef DORMOUSEENGINE_FACTORY_STORAGE_VOLATILE_HPP_
#define DORMOUSEENGINE_FACTORY_STORAGE_VOLATILE_HPP_

#include <memory>
#include <functional>

#include "detail/InstanceType.hpp"
#include "detail/Mapping.hpp"

namespace dormouse_engine::factory::storage {

template <class IdentifierType, class InstanceType>
class Volatile :
	public detail::Mapping<
		IdentifierType,
		std::weak_ptr<detail::InstanceTypeT<InstanceType>>
		>
{
private:

	using StoredType = std::weak_ptr<detail::InstanceTypeT<InstanceType>>;

	using Super = Mapping<
		IdentifierType,
		StoredType
		>;

public:

	using Instance = std::shared_ptr<detail::InstanceTypeT<InstanceType>>;

	using IdentifierParam = typename Super::IdentifierParam;

	using Creator = std::function<InstanceType()>;

	Instance get(const IdentifierParam identifier, Creator creator) {
		auto permanent = Super::getStored(identifier).lock();
		if (!permanent) {
			permanent = Instance(creator());
			Super::store(identifier, permanent);
		}

		return permanent;
	}

	bool isStored(const IdentifierParam identifier) const {
		return !Super::getStored(identifier).expired();
	}

};

} // namespace dormouse_engine::factory::storage

#endif /* DORMOUSEENGINE_FACTORY_STORAGE_VOLATILE_HPP_ */
