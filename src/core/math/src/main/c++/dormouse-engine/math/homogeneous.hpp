#ifndef _DORMOUSEENGINE_MATH_HOMOGENEOUS_HPP_
#define _DORMOUSEENGINE_MATH_HOMOGENEOUS_HPP_

#include "dormouse-engine/essentials/Null.hpp"
#include "Vector.hpp"
#include "ScalarEqual.hpp"

namespace dormouse_engine::math {

template <template<class> class BasisT>
class HomogeneousCoordinates {
public:

	using Basis = BasisT<essentials::Null>;

	constexpr HomogeneousCoordinates(Vec4 vector) noexcept :
		vector_(std::move(vector))
	{
	}

	constexpr operator Vec4() const noexcept {
		return vector_;
	}

	constexpr operator const Vec4&() const noexcept {
		return vector_;
	}

	Vec3 to3dSpace() const noexcept {
		if (ScalarEqual<float>()(vector_.w(), 0.0f)) {
			return vector_.xyz();
		} else {
			return vector_.xyz() / vector_.w();
		}
	}

	constexpr const Vec4& vector() const noexcept {
		return vector_;
	}

	Vec4& vector() noexcept {
		return vector_;
	}

private:

	Vec4 vector_;

};

template <template<class> class BasisT>
class HomogeneousPoint : public HomogeneousCoordinates<BasisT> {
public:

	explicit constexpr HomogeneousPoint(std::initializer_list<float> coords) noexcept :
		HomogeneousPoint(Vec3(std::move(coords)))
	{
	}

	explicit constexpr HomogeneousPoint(float x, float y, float z) noexcept :
		HomogeneousCoordinates(Vec4(x, y, z, 1.0f))
	{
	}

	explicit constexpr HomogeneousPoint(Vec3 point) noexcept :
		HomogeneousCoordinates(Vec4(point, 1.0f))
	{
	}

};

template <template<class> class BasisT>
class HomogeneousVector : public HomogeneousCoordinates<BasisT> {
public:

	explicit constexpr HomogeneousVector(std::initializer_list<float> coords) noexcept :
		HomogeneousVector(Vec3(std::move(coords)))
	{
	}

	explicit constexpr HomogeneousVector(float x, float y, float z) noexcept :
		HomogeneousCoordinates(Vec4(x, y, z, 1.0f))
	{
	}

	explicit constexpr HomogeneousVector(Vec3 vector) noexcept :
		HomogeneousCoordinates(Vec4(vector, 0.0f))
	{
	}

};

} // namespace dormouse_engine::math

#endif /* _DORMOUSEENGINE_MATH_HOMOGENEOUS_HPP_ */
