#ifndef DORMOUSEENGINE_FACTORY_STORAGE_PERMANENT_HPP_
#define DORMOUSEENGINE_FACTORY_STORAGE_PERMANENT_HPP_

#include <functional>
#include <memory>

#include "detail/InstanceType.hpp"
#include "detail/Mapping.hpp"

namespace dormouse_engine {
namespace factory {
namespace storage {

template <class IdentifierType, class InstanceType>
class Permanent :
	public detail::Mapping<
		IdentifierType,
		std::shared_ptr<detail::InstanceTypeT<InstanceType>>
		>
{
private:

	using StoredType = std::shared_ptr<detail::InstanceTypeT<InstanceType>>;

	using Super = Mapping<
		IdentifierType,
		StoredType
		>;

public:

	using Instance = std::shared_ptr<detail::InstanceTypeT<InstanceType>>;

	using IdentifierParam = typename Super::IdentifierParam;

	using Creator = std::function<InstanceType()>;

	Instance get(const IdentifierParam identifier, Creator creator) {
		auto permanent = Super::getStored(identifier);
		if (!permanent) {
			permanent = Super::store(identifier, creator());
		}
		return permanent;
	}

	bool isStored(const IdentifierParam identifier) const {
		return static_cast<bool>(Super::getStored(identifier));
	}

};

} // namespace storage
} // namespace factory
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_FACTORY_STORAGE_PERMANENT_HPP_ */
