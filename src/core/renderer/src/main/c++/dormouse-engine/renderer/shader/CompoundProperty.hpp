#ifndef _DORMOUSEENGINE_RENDERER_SHADER_COMPOUNDPROPERTY_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_COMPOUNDPROPERTY_HPP_

#include <unordered_map>

#include "dormouse-engine/essentials/StringId.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "Property.hpp"

namespace dormouse_engine::renderer::shader {

class CompoundProperty {
public:

	using Properties = std::unordered_map<essentials::StringId, Property>;

	CompoundProperty(essentials::observer_ptr<Properties> properties) :
		properties_(std::move(properties))
	{
	}

	Property get(essentials::StringId id) const {
		auto it = properties_->find(std::move(id));
		if (it == properties_->end()) {
			throw PropertyNotBound(id);
		}
		return it->second;
	}

private:

	essentials::observer_ptr<Properties> properties_;

};

Property getShaderProperty(const CompoundProperty& compoundProperty, essentials::StringId id) {
	return compoundProperty.get(std::move(id));
}

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_COMPOUNDPROPERTY_HPP_ */
