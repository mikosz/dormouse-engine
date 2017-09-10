#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PROPERTYID_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PROPERTYID_HPP_

#include <vector>
#include <iosfwd>
#include <cassert>
#include <algorithm>

#include "dormouse-engine/essentials/InfixOstreamIterator.hpp"
#include "dormouse-engine/essentials/StringId.hpp"

namespace dormouse_engine::renderer::shader {

// TODO: rename PropertyDescriptor to PropertyId, PropertyId to something else (PropertyIdView?)
class PropertyDescriptor {
public:

	struct Object {
		essentials::StringId name;
		size_t arraySize;
		size_t arrayElementOffset;

		Object(essentials::StringId name, size_t arraySize = 0, size_t arrayElementOffset = 0) :
			name(std::move(name)),
			arraySize(arraySize),
			arrayElementOffset(arrayElementOffset)
		{
		}

	};

	using Objects = std::vector<Object>;

	PropertyDescriptor() noexcept = default;

	PropertyDescriptor(Objects objects) noexcept :
		objects_(std::move(objects))
	{
	}

	const Objects& objects() const noexcept {
		return objects_;
	}

private:

	Objects objects_;

};

inline std::ostream& operator<<(std::ostream& os, const PropertyDescriptor::Object& object) {
	os << object.name.string();
	if (object.arraySize > 0) {
		os << "[" << object.arraySize << "+" << object.arrayElementOffset << "]";
	}
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const PropertyDescriptor& propertyDescriptor) {
	std::copy(
		propertyDescriptor.objects().begin(),
		propertyDescriptor.objects().end(),
		essentials::InfixOstreamIterator<PropertyDescriptor::Object>(os, ".")
		);
	return os;
}

class PropertyId {
public:

	PropertyId(const PropertyDescriptor& descriptor) noexcept :
		objectsIt_(descriptor.objects().begin()),
		objectsEnd_(descriptor.objects().end())
	{
	}

	bool empty() const noexcept {
		return objectsIt_ == objectsEnd_;
	}

	const PropertyDescriptor::Object& head() const noexcept {
		assert(!empty());
		return *objectsIt_;
	}

	PropertyId tail() const noexcept {
		assert(!empty());
		return PropertyId(objectsIt_ + 1, objectsEnd_);
	}

private:

	const PropertyDescriptor::Objects::const_iterator objectsIt_;

	const PropertyDescriptor::Objects::const_iterator objectsEnd_;

	PropertyId(
		PropertyDescriptor::Objects::const_iterator objectsIt,
		PropertyDescriptor::Objects::const_iterator objectsEnd
		) noexcept :
		objectsIt_(std::move(objectsIt)),
		objectsEnd_(std::move(objectsEnd))
	{
	}

	friend inline std::ostream& operator<<(std::ostream& os, PropertyId propertyId) {
		std::copy(
			propertyId.objectsIt_,
			propertyId.objectsEnd_,
			dormouse_engine::InfixOstreamIterator<PropertyDescriptor::Object>(os, ".")
			);
		return os;
	}

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PROPERTYID_HPP_ */
