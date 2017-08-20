#ifndef DORMOUSEENGINE_FACTORY_STORAGE_FREEZER_HPP_
#define DORMOUSEENGINE_FACTORY_STORAGE_FREEZER_HPP_

#include <memory>
#include <list>
#include <functional>
#include <numeric>
#include <unordered_map>

#include <boost/call_traits.hpp>

#include "detail/InstanceType.hpp"

namespace dormouse_engine::factory::storage {

// TODO: Freezer is now broken, i.e. doesn't fulfill the current requirements of
// Factory on storage implementations. Thing is I don't use it at the moment.
template <class IdentifierType, class InstanceType>
class Freezer {
public:

	using Identifier = IdentifierType;

	using IdentifierParam = typename boost::call_traits<Identifier>::param_type;

	using Instance = std::shared_ptr<detail::InstanceTypeT<InstanceType>>;

	using SizeFunc = std::function<size_t (const detail::InstanceTypeT<Instance>&)>;

	void setMaxFreezerSize(size_t size) {
		maxSize_ = size;
		cleanup();
	}

	void setSizeFunc(SizeFunc sizeFunc) {
		sizeFunc_ = std::move(sizeFunc);
		totalSize_ = std::accumulate(
			storage_.begin(),
			storage_.end(),
			static_cast<size_t>(0),
			[this](auto value, const auto& entry) {
				return sizeFunc_(*entry.second.data) + value;
			});
		cleanup();
	}

	Instance get(const IdentifierParam identifier) const {
		typename Storage::const_iterator it = storage_.find(identifier);
		if (it == storage_.end()) {
			return Instance();
		} else {
			queue_.splice(queue_.end(), queue_, it->second.queueIt);
			return Instance(it->second.data);
		}
	}

	bool isStored(const IdentifierParam identifier) const {
		return storage_.count(identifier) != 0;
	}

	Instance store(const IdentifierParam identifier, std::unique_ptr<typename Instance::element_type>&& instance) {
		if (isStored(identifier)) {
			erase(identifier);
		}
		Instance permanent(instance.release());

		StorageEntry entry;
		entry.data = permanent;
		entry.queueIt = queue_.emplace(queue_.end(), identifier);

		storage_.emplace(std::make_pair(identifier, std::move(entry)));
		if (sizeFunc_) {
			totalSize_ += sizeFunc_(*permanent);
		}

		cleanup();

		return permanent;
	}

	void erase(const IdentifierParam identifier) {
		auto storageIt = storage_.find(identifier);
		if (storageIt != storage_.end()) {
			if (sizeFunc_) {
				totalSize_ -= sizeFunc_(*storageIt->second.data);
			}
			queue_.erase(storageIt->second.queueIt);
			storage_.erase(storageIt);
		}
	}

	void clear() {
		storage_.clear();
		queue_.clear();
		totalSize_ = 0u;
	}

private:

	using Stored = std::shared_ptr<InstanceType>;

	using Queue = std::list<Identifier>;

	struct StorageEntry {

		Stored data;

		typename Queue::iterator queueIt;

	};

	using Storage = std::unordered_map<Identifier, StorageEntry>;

	Storage storage_;

	mutable Queue queue_;

	SizeFunc sizeFunc_;

	size_t totalSize_ = 0u;

	size_t maxSize_ = 0u;

	void cleanup() {
		if (maxSize_ == 0) {
			return;
		}

		while (totalSize_ > maxSize_) {
			erase(queue_.front());
		}
	}

};

} // namespace dormouse_engine::factory::storage

#endif /* DORMOUSEENGINE_FACTORY_STORAGE_FREEZER_HPP_ */
