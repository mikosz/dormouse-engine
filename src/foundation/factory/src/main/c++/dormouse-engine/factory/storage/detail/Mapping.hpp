#ifndef DORMOUSEENGINE_FACTORY_STORAGE_DETAIL_MAPPING_HPP_
#define DORMOUSEENGINE_FACTORY_STORAGE_DETAIL_MAPPING_HPP_

#include <algorithm>
#include <memory>
#include <unordered_map>

#include <boost/call_traits.hpp>

namespace dormouse_engine::factory::storage::detail {

template <class IdentifierType, class StoredType>
class Mapping {
public:

	using Identifier = IdentifierType;

	using IdentifierParam = typename boost::call_traits<Identifier>::param_type;

	StoredType store(const IdentifierParam identifier, StoredType stored) {
		storage_.erase(identifier);
		return storage_.emplace(identifier, std::move(stored)).first->second;
	}

	void erase(const IdentifierParam identifier) {
		storage_.erase(identifier);
	}

	void clear() {
		storage_.clear();
	}

protected:

	using Stored = StoredType;

	Stored getStored(const IdentifierParam identifier) const {
		typename Storage::const_iterator it = storage_.find(identifier);
		if (it == storage_.end()) {
			return Stored();
		} else {
			return Stored(it->second);
		}
	}

private:

	using Storage = std::unordered_map<Identifier, Stored>;

	Storage storage_;

};

} // namespace dormouse_engine::factory::storage::detail

#endif /* DORMOUSEENGINE_FACTORY_STORAGE_DETAIL_MAPPING_HPP_ */
