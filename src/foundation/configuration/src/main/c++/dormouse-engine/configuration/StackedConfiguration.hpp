#ifndef DORMOUSEENGINE_CONFIGURATION_STACKEDCONFIGURATION_HPP_
#define DORMOUSEENGINE_CONFIGURATION_STACKEDCONFIGURATION_HPP_

#include <deque>

#include <functional>

#include <boost/unordered_set.hpp>

#include "dormouse-engine/essentials/pointee.hpp"
#include "StringConfiguration.hpp"

namespace dormouse_engine {
namespace configuration {

/**
 * StackedConfiguration stores the configuration in layers. Layers on top of others override values specified
 * on lower levels. This functionality may be used to create a configuration that is loaded from several
 * sources, for instance: default values < global file < local file < command line < environment variables.
 * All write operations on the configuration are performed on the topmost layer. As a result, if the layer is
 * popped, all changes are discarded. Read operations are performed top-down. If a value is specified or
 * erased on the current level, the search is stopped and the value(s) returned.
 */
template <class KeyType, class ValueType>
class StackedConfiguration : public detail::StringConfigurationSuperclassTraits<KeyType, ValueType>::Super {
private:

	typedef typename detail::StringConfigurationSuperclassTraits<KeyType, ValueType>::Super Super;

public:

	void push(typename Super::Ptr layer) {
		stack_.push_back(layer);
	}

	void pop() {
		// TODO: underflow?
		stack_.pop_back();
	}

	void flatten(typename Super::Ptr target) {
		typename Super::Keys k;
		keys(&k);
		std::for_each(k.begin(), k.end(), std::bind(&copyAll, this, std::placeholders::_1, target));
		stack_.clear();
		push(target);
	}

	void clear() {
		Layer& top = stack_.back();
		typename Super::Keys k;
		top.configuration_->keys(&k);
		std::copy(k.begin(), k.end(), std::inserter(top.removed_, top.removed_.end()));
		top.configuration_->clear();
	}

	bool empty() const {
		typename Super::Keys k;
		keys(&k);
		return k.empty();
	}

	size_t count(const typename Super::KeyParam key) const {
		return firstWith<size_t>(
				key,
				0,
				std::bind(&Configuration<typename Super::Key, typename Super::Value>::count, std::placeholders::_1, key)
		);
	}

	typename Super::Value get(const typename Super::KeyParam key) const {
		typename Super::Values values;
		getAll(key, &values);
		if (values.empty()) {
			throw MissingRequiredValue(boost::lexical_cast<std::string>(key));
		} else if (values.size() > 1) {
			throw MultipleValuesWhereSingleValueRequired(boost::lexical_cast<std::string>(key));
		} else {
			return values.front();
		}
	}

	void getAll(
			const typename Super::KeyParam key,
			typename Super::Values* values
			) const {
		firstWith(
				key,
				std::bind(
						&Configuration<typename Super::Key, typename Super::Value>::getAll,
						std::placeholders::_1,
						std::cref(key),
						values
						)
		);
	}

	void set(const typename Super::KeyParam key, const typename Super::ValueParam value) {
		// TODO: underflow?
		stack_.back().removed_.erase(key);
		stack_.back().configuration_->set(key, value);
	}

	void add(const typename Super::KeyParam key, const typename Super::ValueParam value) {
		// TODO: underflow?
		stack_.back().removed_.erase(key);
		stack_.back().configuration_->add(key, value);
	}

	void erase(const typename Super::KeyParam key) {
		// TODO: underflow?
		stack_.back().configuration_->erase(key);
		stack_.back().removed_.insert(key);
	}

	void keys(typename Super::Keys* keysPtr) const {
		typename Super::Keys& keys = essentials::pointee(keysPtr);

		typename Stack::const_iterator it, end = stack_.end();
		for (it = stack_.begin(); it != end; ++it) {
			typename Layer::Removed::const_iterator removedIt, removedEnd = it->removed_.end();
			for (removedIt = it->removed_.begin(); removedIt != removedEnd; ++removedIt) {
				keys.erase(*removedIt);
			}
			it->configuration_->keys(&keys);
		}
	}

private:

	struct Layer {
	public:

		typedef boost::unordered_set<typename Super::Key> Removed;

		Layer(typename Super::Ptr configuration) :
			configuration_(configuration) {
		}

		typename Super::Ptr configuration_;

		Removed removed_;

	};

	typedef std::deque<Layer> Stack;

	Stack stack_;

	template <class ReturnType>
	ReturnType firstWith(
			const typename Super::KeyParam key,
			ReturnType defaultValue,
			std::function<ReturnType(typename Super::Ptr)> f
	) const {
		typename Stack::const_iterator it, end = stack_.end();
		for (it = stack_.begin(); it != end; ++it) {
			if (it->configuration_->count(key)) {
				return f(it->configuration_);
			} else if (it->removed_.count(key)) {
				break;
			}
		}
		return defaultValue;
	}

	void firstWith(
			const typename Super::KeyParam key,
			std::function<void(typename Super::Ptr)> f
	) const {
		typename Stack::const_iterator it, end = stack_.end();
		for (it = stack_.begin(); it != end; ++it) {
			if (it->configuration_->count(key)) {
				f(it->configuration_);
				return;
			} else if (it->removed_.count(key)) {
				break;
			}
		}
	}

	void copyAll(typename Super::Key key, typename Super::Ptr target) {
		typename Super::Values values;
		getAll(key, &values);
		std::for_each(
				values.begin(),
				values.end(),
				std::bind(&Configuration<typename Super::Key, typename Super::Value>::add, target, std::placeholders::_1)
		);
	}

};

}  // namespace configuration
}  // namespace dormouse_engine

#endif /* DORMOUSEENGINE_CONFIGURATION_STACKEDCONFIGURATION_HPP_ */
