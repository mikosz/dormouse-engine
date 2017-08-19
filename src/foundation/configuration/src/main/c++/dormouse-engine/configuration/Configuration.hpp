#ifndef DORMOUSEENGINE_CONFIGURATION_CONFIGURATION_HPP_
#define DORMOUSEENGINE_CONFIGURATION_CONFIGURATION_HPP_

#include <vector>
#include <memory>
#include <unordered_set>

#include <boost/lexical_cast.hpp>
#include <boost/call_traits.hpp>
#include <boost/ref.hpp>
#include <boost/optional.hpp>

#include "configuration-exceptions.hpp"

namespace dormouse_engine {
namespace configuration {

/**
 * Generic configuration interface. Design to support both simple key-value configurations as well as
 * structured configurations, such as ones created by xml documents.
 */
template <class KeyType, class ValueType>
class Configuration {
public:

	typedef KeyType Key;

	typedef ValueType Value;

	typedef typename boost::call_traits<Key>::param_type KeyParam;

	typedef typename boost::call_traits<Value>::param_type ValueParam;

	typedef std::unordered_set<Key> Keys;

	typedef std::vector<Value> Values;

	typedef std::shared_ptr<Configuration<Key, Value> > Ptr;

	typedef std::shared_ptr<const Configuration<Key, Value> > ConstPtr;

	virtual ~Configuration() {
	}

	/**
	 * Clear the configuration.
	 *
	 * Postconditions: empty() == true
	 */
	virtual void clear() = 0;

	/**
	 * Inform whether the configuration is empty.
	 *
	 * @return bool - true iff for all keys k count(k) == 0.
	 */
	virtual bool empty() const = 0;

	/**
	 * Returns the number of occurences of the provided key in the configuration.
	 *
	 * @param key - the key to look for in the configuration
	 * @return size_t - the number of occurences of key in the configuration
	 */
	virtual size_t count(KeyParam key) const = 0;

	/**
	 * Returns a single value specified for the provided key. If none or multiple values are specified,
	 * throws a relevant exception.
	 *
	 * @param key - the key to look for in the configuration
	 * @return Value - the value specified for key
	 * @throws MissingRequiredValue - iff count(key) == 0
	 * @throws MultipleValuesWhereSingleValueRequired - iff count(key) > 0
	 */
	virtual Value get(KeyParam key) const = 0;

	/**
	 * Returns all values specified for the provided key. Does not clear values beforehand.
	 *
	 * @param key - the key to look for in the configuration
	 * @param values[out] - all values specified for key
	 */
	virtual void getAll(KeyParam key, Values* values) const = 0;

	/**
	 * Sets the value for the provided key, overwriting the existing values.
	 *
	 * Postcondition: get(key) == value
	 */
	virtual void set(KeyParam key, const ValueParam value) = 0;

	/**
	 * Adds another value for the provided key.
	 *
	 * Precondition: count(key) == n
	 * Postcondition: count(key) == n + 1 and getAll(key) contains value
	 */
	virtual void add(KeyParam key, const ValueParam value) = 0;

	/**
	 * Remove all occurences of the provided key.
	 *
	 * Postcondition: count(key) == 0
	 *
	 * @param key - the key to look for in the configuration
	 */
	virtual void erase(KeyParam key) = 0;

	/**
	 * Returns all the keys existing in the current configuration. Does not clear k beforehand.
	 *
	 * @param k[out] - all keys in the current configuration
	 */
	virtual void keys(Keys* k) const = 0;

	/**
	 * Returns the value specified for the given key or defaultValue if no value is specified.
	 *
	 * @param key - the key to look for in the configuration
	 * @param defaultValue - the defaultValue which will be returned if count(key) == 0
	 * @return boost::optional<Value> - the value specified for key, or defaultValue if count(key) == 0
	 * @throws MultipleValuesWhereSingleValueRequired - iff count(key) > 0
	 */
	template <class OptionalType = boost::optional<Value> >
	OptionalType getOptional(
			KeyParam key,
			OptionalType defaultValue = OptionalType()
			) const {
		return count(key) ? get(key) : defaultValue;
	}

	/**
	* Returns the value specified for the given key lexical cast to the required type.
	*
	* @param Target - the target type
	* @param key - the key to look for in the configuration
	* @return Target - the value specified for key parsed as Target
	* @throws MissingRequiredValue - iff count(key) == 0
	* @throws MultipleValuesWhereSingleValueRequired - iff count(key) > 0
	* @throws BadValueType - iff get(key) is not parsable as T
	* 
	* FIXME: does this belong here? StringConfiguration has this, and this is probably only correct in that context, delete this or the other
	*/
	template <class Target>
	Target getAs(KeyParam key) const {
		try {
			return boost::lexical_cast<Target>(get(key));
		} catch (const boost::bad_lexical_cast&) {
			throw BadValueType(boost::lexical_cast<Key>(key), boost::lexical_cast<Value>(get(key)), static_cast<Target*>(nullptr));
		}
	}

};

} // namespace configuration
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_CONFIGURATION_CONFIGURATION_HPP_ */
