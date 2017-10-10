#include "Parameter.hpp"

#include "dormouse-engine/essentials/Range.hpp"
#include "Property.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer::shader;

namespace /* anonymous */ {

void writeProperty(
	const Property& property,
	PropertyId id,
	essentials::BufferView buffer,
	const graphics::ShaderDataType& dataType
	)
{
	if (!id.empty()) {
		for (auto idx : essentials::range<size_t>(0u, std::max<size_t>(1u, id.head().arraySize))) {
			if (!property.has(id.head().name, idx)) {
				throw PropertyNotBound(id);
			}

			writeProperty(
				property.get(id.head().name, idx),
				id.tail(),
				buffer + idx * id.head().arrayElementOffset,
				dataType
				);
		}
	} else {
		property.write(buffer, dataType);
	}
}

} // anonymous namespace

void Parameter::write(essentials::BufferView buffer, const Property& root) const {
	// TODO: duplicated with detail::ShaderBase::bindResource_, could be done by Property,
	// or PropertyDescriptor
	auto id = PropertyId(propertyDescriptor_);

	assert(!id.empty());

	writeProperty(root, id, buffer + offset_, dataType_);
}
