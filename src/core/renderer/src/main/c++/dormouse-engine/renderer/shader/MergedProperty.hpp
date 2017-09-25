#ifndef _DORMOUSEENGINE_RENDERER_SHADER_MERGEDPROPERTY_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_MERGEDPROPERTY_HPP_

#include "dormouse-engine/essentials/StringId.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "Property.hpp"

namespace dormouse_engine::renderer::shader {

class MergedProperty {
public:

	MergedProperty(
		essentials::observer_ptr<const Property> added,
		essentials::observer_ptr<const Property> original
		) :
		added_(std::move(added)),
		original_(std::move(original))
	{
	}

	bool has(essentials::StringId id, size_t arrayIdx) const {
		return added_->has(id, arrayIdx) || original_->has(id, arrayIdx);
	}

	Property get(essentials::StringId id, size_t arrayIdx) const {
		if (added_->has(id)) {
			return added_->get(std::move(id), arrayIdx);
		}
		return original_->get(std::move(id), arrayIdx);
	}

private:

	essentials::observer_ptr<const Property> added_;

	essentials::observer_ptr<const Property> original_;

};

bool hasShaderProperty(const MergedProperty& mergedProperty, essentials::StringId id, size_t arrayIdx) {
	return mergedProperty.has(std::move(id), arrayIdx);
}

Property getShaderProperty(const MergedProperty& mergedProperty, essentials::StringId id, size_t arrayIdx) {
	return mergedProperty.get(std::move(id), arrayIdx);
}

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_MERGEDPROPERTY_HPP_ */
