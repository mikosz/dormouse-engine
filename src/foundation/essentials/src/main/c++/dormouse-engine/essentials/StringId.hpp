#ifndef DORMOUSEENGINE_ESSENTIALS_STRINGID_HPP_
#define DORMOUSEENGINE_ESSENTIALS_STRINGID_HPP_

#include <cassert>
#include <string>
#include <unordered_map>
#include <cstdint>

#include <boost/operators.hpp>

#include "dormouse-engine/exceptions/RuntimeError.hpp"
#include "Singleton.hpp"
#include "functional.hpp"

namespace dormouse_engine::essentials {

class ClashingStringIdHash : public exceptions::RuntimeError {
public:

	ClashingStringIdHash(std::string stored, std::string added);

};

class StringId : boost::equality_comparable<StringId> {
public:

	StringId() noexcept = default;

	StringId(std::string s);

	StringId(const char* cs) :
		StringId(std::string(cs))
	{
	}

	const std::string& string() const noexcept;

private:

	using Hash = std::size_t;

	static const auto INVALID_HASH = static_cast<Hash>(-1);

	Hash hash_ = INVALID_HASH;

	StringId(Hash hash) :
		hash_(hash)
	{
	}

	friend bool operator==(StringId lhs, StringId rhs) {
		return lhs.hash_ == rhs.hash_;
	}

	friend class Strings;

	friend struct std::hash<StringId>;

};

class Strings : public essentials::Singleton<Strings> {
public:

	StringId add(std::string s);

private:

	using Registry = std::unordered_map<StringId::Hash, std::string, Identity>;

	Registry registry_;

	const std::string& get(StringId::Hash hash) const {
		const auto it = registry_.find(hash);
		assert(it != registry_.end());
		return it->second;
	}

	friend class StringId;

};

inline StringId::StringId(std::string s) :
	StringId(Strings::instance()->add(std::move(s)))
{
}

inline const std::string& StringId::string() const noexcept {
	assert(hash_ != INVALID_HASH);
	return Strings::instance()->get(hash_);
}

} // namespace dormouse_engine::essentials

namespace std {

template <>
struct hash<dormouse_engine::essentials::StringId> {

	auto operator()(const dormouse_engine::essentials::StringId& stringId) const noexcept {
		return stringId.hash_;
	}

};

} // namespace std

#endif /* DORMOUSEENGINE_ESSENTIALS_STRINGID_HPP_ */
