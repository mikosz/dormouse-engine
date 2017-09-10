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

	bool has(essentials::StringId id) const {
		return added_->has(id) || original_->has(id);
	}

	Property get(essentials::StringId id) const {
		if (added_->has(id)) {
			return added_->get(std::move(id));
		}
		return original_->get(std::move(id));
	}

private:

	essentials::observer_ptr<const Property> added_;

	essentials::observer_ptr<const Property> original_;

};

bool hasShaderProperty(const MergedProperty& mergedProperty, essentials::StringId id) {
	return mergedProperty.has(std::move(id));
}

Property getShaderProperty(const MergedProperty& mergedProperty, essentials::StringId id) {
	return mergedProperty.get(std::move(id));
}

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_MERGEDPROPERTY_HPP_ */
