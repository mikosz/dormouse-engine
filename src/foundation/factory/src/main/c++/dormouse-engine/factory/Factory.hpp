#ifndef DORMOUSEENGINE_FACTORY_FACTORY_HPP_
#define DORMOUSEENGINE_FACTORY_FACTORY_HPP_

#include <unordered_map>

#include <boost/call_traits.hpp>

namespace dormouse_engine {
namespace factory {

/**
 * A generic implementation of the "Factory" design pattern.
 * 
 * @param IdentifierType Type of the parameter identifying objects to be created
 * @param CreatorType A policy class from which Factory publicly inherits. It may provide additional functions
 *	 to be exposed for the user. Required to implement a doCreate(IdentifierType, ...) function, that returns
 *	 a newly created object and to define an Instance type that is the result of this function.
 * @param StorageType A policy class that stores created objects. Requirements for this class are:
 *	 * default-constructible
 *	 * defines an Instance type - this is the final type that will be returned by the create() function
 *	 * implements a isStored(IdentifierType) function, returning true iff it is capable of returning an
 *	   object with the provided identifier
 *	 * implements a store(IdentifierType, decltype(CreatorType::doCreate(...))) function that stores the
 *	   provided object and returns it as a StorageType::Instance
 *	 * implements a get(IdentifierType) that returns a stored StorageType::Instance
 * @param MutexType Type of a the mutex that will be used to achieve thread safety. Must be compatible with
 *	 concurrent::Lockable's MutexType parameter.
 */
template <
	class IdentifierType,
	class CreatorType,
	// TODO: instead of CreatorType::InstanceType, shouldn't storages take a CreatorFunc parameter?
	template<class /* IdentifierType */, class /* CreatorType::InstanceType */> class StorageType
	>
class Factory : public CreatorType
{
public:

	using Identifier = IdentifierType;

	using IdentifierParam = typename boost::call_traits<Identifier>::param_type;

	using Storage = StorageType<Identifier, typename CreatorType::Instance>;

	using StoredInstance = typename Storage::Instance;

	template <class... CreatorParams>
	Factory(CreatorParams&&... creatorParams) :
		CreatorType(std::forward<CreatorParams>(creatorParams)...)
	{
	}

	template <class... CreatorParams>
	StoredInstance create(const IdentifierParam id, CreatorParams&&... creatorParams) {
		return storage_.get(
			id,
			[this, &creatorParams..., &id]() {
					return doCreate(id, std::forward<CreatorParams>(creatorParams)...);
				});
	}

	template <class... CreatorParams>
	StoredInstance create(const IdentifierParam id, CreatorParams&&... creatorParams) volatile {
		return lock()->create(id, std::forward<CreatorParams>(creatorParams)...);
	}

	const Storage& storage() const {
		return storage_;
	}

	Storage& storage() {
		return storage_;
	}

private:

	Storage storage_;

};

} // namespace factory
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_FACTORY_FACTORY_HPP_ */
