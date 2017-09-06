#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_

#include <memory>
#include <cassert>

#include "dormouse-engine/essentials/StringId.hpp"
#include "dormouse-engine/graphics/ShaderType.hpp"
#include "../control/ResourceView.hpp"

namespace dormouse_engine::renderer::shader {

class Properties;

class PropertyObject {
public:

	PropertyObject() = default;

	template <class T>
	PropertyObject(T model) :
		object_(std::make_shared<Model<T>>(std::move(model)))
	{
	}

	control::ResourceView resource(essentials::StringId id) const {
		return object_->resource(id);
	}

	const Properties& properties() const {
		return object_->properties();
	}

private:

	class Concept {
	public:

		virtual ~Concept() = default;

		virtual control::ResourceView resource(essentials::StringId id) const = 0;
			
		virtual const Properties& properties() const = 0;

	};

	template <class T>
	class Model : public Concept {
	public:

		//virtual control::ResourceView resource(essentials::StringId id) const override {
		//	return shaderObjectResource(std::move(id));
		//}

		//virtual const Properties& properties() const override {
		//	return shaderObjectProperties(model_);
		//}

	private:

		T model_;

	};

	std::shared_ptr<const Concept> object_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_ */
