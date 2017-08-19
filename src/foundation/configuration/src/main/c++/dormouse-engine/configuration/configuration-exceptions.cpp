#include "configuration-exceptions.hpp"

#include <sstream>

using namespace dormouse_engine::configuration;

std::string MissingRequiredValue::constructMessage(const std::string& key) {
	std::ostringstream oss;
	oss << "Missing required value in configuration for key \"" << key << "\"";
	return oss.str();
}

std::string MultipleValuesWhereSingleValueRequired::constructMessage(const std::string& key) {
	std::ostringstream oss;
	oss << "Multiple values found where single value was required for key \"" << key << "\"";
	return oss.str();
}

std::string ConfigurationValueNotUpdateable::constructMessage(const std::string& key,
		const std::string& oldValue, const std::string& newValue) {
	std::ostringstream oss;
	oss << "Invalid configuration update for key \"" << key << "\" "
			"from value \"" << oldValue << "\" "
			"to value\"" << newValue << "\"";
	return oss.str();
}
