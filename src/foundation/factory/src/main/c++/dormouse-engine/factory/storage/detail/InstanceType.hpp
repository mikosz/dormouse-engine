#ifndef DORMOUSEENGINE_FACTORY_STORAGE_DETAIL_INSTANCETYPE_HPP_
#define DORMOUSEENGINE_FACTORY_STORAGE_DETAIL_INSTANCETYPE_HPP_

#include <memory>

namespace dormouse_engine {
namespace factory {
namespace storage {
namespace detail {

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

} // namespace detail
} // namespace storage
} // namespace factory
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_FACTORY_STORAGE_DETAIL_INSTANCETYPE_HPP_ */
