#ifndef _DORMOUSEENGINE_MATH_PERLINNOISE_HPP_
#define _DORMOUSEENGINE_MATH_PERLINNOISE_HPP_

#include <vector>

#include "coconut/pulp/math/Vector.hpp"

namespace coconut::pulp::math {

// TODO: put me somewhere else

/*
 * Perlin-noise implementation based on https://github.com/sol-prog/Perlin_Noise
 */
class PerlinNoise {
public:

	PerlinNoise(unsigned int seed);

	float sample(const Vec3& where) const;

private:

	std::vector<size_t> data_;

};

} // namespace coconut::pulp::math

#endif /* _DORMOUSEENGINE_MATH_PERLINNOISE_HPP_ */
