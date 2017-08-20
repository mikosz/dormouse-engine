#ifndef DORMOUSEENGINE_UTILS_HASH_COMBINE_HPP_
#define DORMOUSEENGINE_UTILS_HASH_COMBINE_HPP_

#include <functional>

namespace dormouse_engine::essentials {

inline size_t hashCombine(size_t seed, size_t value) {
	// Code from boost
	// Reciprocal of the golden ratio helps spread entropy and handles duplicates.
	// See Mike Seymour in magic-numbers-in-boosthash-combine: http://stackoverflow.com/questions/4948780
	
	seed ^= std::hash_value(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2); // TODO: hash_value doesn't exist, why does this work?
	return seed;
}

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_UTILS_HASH_COMBINE_HPP_ */
