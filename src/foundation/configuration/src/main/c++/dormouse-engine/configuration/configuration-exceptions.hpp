#ifndef DORMOUSEENGINE_CONFIGURATION_CONFIGURATION_EXCEPTIONS_HPP_
#define DORMOUSEENGINE_CONFIGURATION_CONFIGURATION_EXCEPTIONS_HPP_

#include <typeinfo>

#include <boost/call_traits.hpp>

#include "dormouse-engine/exceptions/RuntimeError.hpp"

namespace dormouse_engine::configuration {

class ConfigurationException : public exceptions::RuntimeError {
public:

	EXCEPTION_CONSTRUCTORS(ConfigurationException, exceptions::RuntimeError)

};

class MissingRequiredValue : public ConfigurationException {
public:

	MissingRequiredValue(const std::string& theKey) :
		ConfigurationException(constructMessage(theKey)),
		key_(theKey) {
	}

	~MissingRequiredValue() throw() {
	}

	const std::string& key() const {
		return key_;
	}

private:

	std::string key_;

	static std::string constructMessage(const std::string& key);

};

class MultipleValuesWhereSingleValueRequired : public ConfigurationException {
public:

	MultipleValuesWhereSingleValueRequired(const std::string& theKey) :
		ConfigurationException(constructMessage(theKey)),
		key_(theKey) {
	}

	~MultipleValuesWhereSingleValueRequired() throw() {
	}

	const std::string& key() const {
		return key_;
	}

private:

	std::string key_;

	static std::string constructMessage(const std::string& key);

};

class ConfigurationValueNotUpdateable : public ConfigurationException {
public:

	ConfigurationValueNotUpdateable(const std::string& theKey,
			const std::string& theOldValue,
			const std::string& theNewValue) :
			ConfigurationException(constructMessage(theKey, theOldValue, theNewValue)),
			key_(theKey),
			oldValue_(theOldValue),
			newValue_(theNewValue) {
	}

	~ConfigurationValueNotUpdateable() throw() {
	}

	const std::string& key() const {
		return key_;
	}

	const std::string& oldValue() const {
		return oldValue_;
	}

	const std::string& newValue() const {
		return newValue_;
	}

private:

	std::string key_;

	std::string oldValue_;

	std::string newValue_;

	static std::string constructMessage(const std::string& key, const std::string& oldValue,
			const std::string& newValue);

};

class BadValueType : public ConfigurationException {
public:

	template <class T>
	BadValueType(const std::string& key, const std::string& value, T*) :
		ConfigurationException(constructMessage<T>(key, value)),
		key_(key),
		value_(value),
		type_(typeid(T).name()) {
	}

	~BadValueType() throw() {
	}

	const std::string& key() const {
		return key_;
	}

	const std::string& value() const {
		return value_;
	}

	const std::string& type() const {
		return type_;
	}

private:

	template <class T>
	static std::string constructMessage(const std::string& key, const std::string& value) {
		std::ostringstream oss;
		oss << "Value \"" << value << "\" specified for key \"" << key
				<< "\" is incompatible with requested type \"" << typeid(T).name() << "\"";
		return oss.str();
	}

	std::string key_;

	std::string value_;

	std::string type_;

};

} // namespace dormouse_engine::configuration

#endif /* DORMOUSEENGINE_CONFIGURATION_CONFIGURATION_EXCEPTIONS_HPP_ */
