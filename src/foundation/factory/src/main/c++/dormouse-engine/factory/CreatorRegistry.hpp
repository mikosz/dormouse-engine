#ifndef DORMOUSEENGINE_FACTORY_CREATORREGISTRY_HPP_
#define DORMOUSEENGINE_FACTORY_CREATORREGISTRY_HPP_

#include <unordered_map>

#include <boost/call_traits.hpp>

namespace dormouse_engine::factory {

template <
	class IdentifierType,
	class CreationPolicy,
	template<class /* IdentifierType */> class ErrorPolicy
	>
class CreatorRegistry {
public:

	using Identifier = IdentifierType;

	using IdentifierParam = typename boost::call_traits<Identifier>::param_type;

	using Instance = typename CreationPolicy::Instance;

	bool isCreatorRegistered(const IdentifierParam id) const {
		return creators_.count(id) != 0;
	}

	void registerCreator(const IdentifierParam id, CreationPolicy creator) {
		auto it = creators_.lower_bound(id);
		if (it != creators_.end() && it->first == id) {
			ErrorPolicy<Identifier>::alreadyRegistered(id);
		} else {
			creators_.insert(it, std::make_pair(id, creator));
		}
	}

	void unregisterCreator(const IdentifierParam id) {
		if (!creators_.erase(id)) {
			ErrorPolicy<Identifier>::noSuchType(id);
		}
	}

protected:

	template <class... CreateParams>
	typename Instance doCreate(const IdentifierParam id, CreateParams&&... createParams) {
		auto it = creators_.find(id);
		if (it == creators_.end()) {
			ErrorPolicy<Identifier>::noSuchType(id);
			return Instance();
		}
		return it->second.create(std::forward<CreateParams>(createParams)...);
	}

private:

	using CreationPolicies = std::unordered_map<Identifier, CreationPolicy>;

	CreationPolicies creators_;

};

} // namespace dormouse_engine::factory

#endif /* DORMOUSEENGINE_FACTORY_CREATORREGISTRY_HPP_ */
