#ifndef _DORMOUSEENGINE_MATH_BASIS_HPP_
#define _DORMOUSEENGINE_MATH_BASIS_HPP_

#include "dormouse-engine/essentials/Null.hpp"

namespace dormouse_engine::math {

namespace basis {

template <class Parent = essentials::Null>
struct Any : Parent {};

template <class Parent = essentials::Null>
struct Same : Parent {};

template <class Parent = essentials::Null>
struct Local : Parent {};

template <class Parent = essentials::Null>
struct World : Parent {};

template <class Parent = essentials::Null>
struct Camera : Parent {};

template <class Parent = essentials::Null>
struct Screen : Parent {};

struct LocalWithID {
	int id_;
};

} // namespace basis

} // namespace dormouse_engine::math

#endif /* _DORMOUSEENGINE_MATH_BASIS_HPP_ */
