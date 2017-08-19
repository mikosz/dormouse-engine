#ifndef DORMOUSEENGINE_CONFIGURATION_STRINGCONFIGURATION_HPP_
#define DORMOUSEENGINE_CONFIGURATION_STRINGCONFIGURATION_HPP_

#include "Configuration.hpp"

namespace dormouse_engine {
namespace configuration {

template <class StringType>
class StringConfiguration : public Configuration<StringType, StringType> {
private:

	typedef Configuration<StringType, StringType> Super;

public:

	template <class C>
	void getAllAs(C* values, const typename Super::KeyParam key) const {
		std::vector<std::reference_wrapper<const typename Super::Value> > sourceValues;
		getAll(&sourceValues, key);
		std::transform(sourceValues.begin(), sourceValues.end(), std::inserter(*values, values->end()),
				std::bind(&as<typename C::value_type>, key, std::placeholders::_1));
	}

private:

	template <class T>
	static T as(const typename Super::KeyParam key, const typename Super::ValueParam value) {
		try {
			return boost::lexical_cast<T>(value);
		} catch (const boost::bad_lexical_cast&) {
			throw BadValueType(essentials::toString<StringType>(key), essentials::toString<StringType>(value), static_cast<T*>(0));
		}
	}

};

namespace detail {

/**
 * Utility class for selecting a superclass for a configuration class. Configuration implementations may
 * specify StringConfigurationSuperclassTraits<Key, Value>::Super as their superclass. All implementations
 * that use string types as keys and values will then use StringConfiguration as their superclass thus
 * inheriting facilitating functions implemented for such types.
 */
template<class Key, class Value>
class StringConfigurationSuperclassTraits {
public:

	typedef Configuration<Key, Value> Super;

};

template<class Char, class Traits, class Alloc>
class StringConfigurationSuperclassTraits<std::basic_string<Char, Traits, Alloc>, std::basic_string<Char, Traits, Alloc> > {
public:

	typedef StringConfiguration<std::basic_string<Char, Traits, Alloc> > Super;

};

} // namespace detail

}  // namespace configuration
}  // namespace dormouse_engine

#endif /* DORMOUSEENGINE_CONFIGURATION_STRINGCONFIGURATION_HPP_ */
