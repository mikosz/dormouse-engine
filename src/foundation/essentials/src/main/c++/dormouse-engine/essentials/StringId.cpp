#include "StringId.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::essentials;

ClashingStringIdHash::ClashingStringIdHash(std::string stored, std::string added) :
	exceptions::RuntimeError(
		"Clashin StringId hash for. New string: \"" + std::move(added) +
		"\" would overwrite \"" + std::move(stored) + "\""
		)
{
}

StringId Strings::add(std::string s) {
	const auto hash = std::hash<std::string>()(s);
	const auto it = registry_.find(hash);

	if (it != registry_.end() && it->second != s) {
		throw ClashingStringIdHash(it->second, std::move(s));
	} else {
		registry_.emplace_hint(it, hash, std::move(s));
	}

	return hash;
}
