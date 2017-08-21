#ifndef _DORMOUSEENGINE_MATH_COMMON_HPP_
#define _DORMOUSEENGINE_MATH_COMMON_HPP_

namespace coconut::pulp::math {

template <class T>
inline T lerp(T from, T to, float factor) { 
	return (1.0f - factor) * from + factor * to; 
}

} // namespace math

using math::lerp;

} // namespace coconut::pulp

#endif /* _DORMOUSEENGINE_MATH_COMMON_HPP_ */
