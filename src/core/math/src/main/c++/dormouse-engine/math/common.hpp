#ifndef _DORMOUSEENGINE_MATH_COMMON_HPP_
#define _DORMOUSEENGINE_MATH_COMMON_HPP_

namespace dormouse_engine::math {

template <class T>
inline T lerp(T from, T to, float factor) { 
	return (1.0f - factor) * from + factor * to; 
}

} // namespace dormouse_engine::math

namespace dormouse_engine {

using math::lerp;

} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_MATH_COMMON_HPP_ */
