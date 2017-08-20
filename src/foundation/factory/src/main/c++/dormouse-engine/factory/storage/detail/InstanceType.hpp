#ifndef DORMOUSEENGINE_FACTORY_STORAGE_DETAIL_INSTANCETYPE_HPP_
#define DORMOUSEENGINE_FACTORY_STORAGE_DETAIL_INSTANCETYPE_HPP_

#include <memory>

namespace dormouse_engine::factory::storage::detail {

template <class T>
struct InstanceType {
	using Type = T;
};

template <class T>
struct InstanceType<std::unique_ptr<T>> {
	using Type = T;
};

template <class T>
struct InstanceType<std::shared_ptr<T>> {
	using Type = T;
};

template <class T>
struct InstanceType<std::weak_ptr<T>> {
	using Type = T;
};

template <class T>
using InstanceTypeT = typename InstanceType<T>::Type;

} // namespace dormouse_engine::factory::storage::detail

#endif /* DORMOUSEENGINE_FACTORY_STORAGE_DETAIL_INSTANCETYPE_HPP_ */
