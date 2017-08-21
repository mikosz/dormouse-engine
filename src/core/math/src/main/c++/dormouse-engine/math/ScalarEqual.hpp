#ifndef _DORMOUSEENGINE_MATH_SCALAREQUAL_HPP_
#define _DORMOUSEENGINE_MATH_SCALAREQUAL_HPP_

namespace dormouse_engine::math {

template <class ScalarType>
struct ScalarEqual {

	constexpr bool operator()(ScalarType lhs, ScalarType rhs) const noexcept {
		return std::equal_to<ScalarType>()(lhs, rhs);
	}

};

template <>
struct ScalarEqual<float> {

	constexpr bool operator()(float lhs, float rhs) const noexcept {
		return std::abs(lhs - rhs) < 1.0f / 10'000.0f;
	}

};

} // namespace dormouse_engine::math

#endif /* _DORMOUSEENGINE_MATH_SCALAREQUAL_HPP_ */
