#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PROPERTIES_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PROPERTIES_HPP_

#include "dormouse-engine/exceptions/RuntimeError.hpp"
#include "dormouse-engine/essentials/StringId.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"

namespace dormouse_engine::renderer::shader {

class PropertyNotBound : public exceptions::RuntimeError {
public:

	PropertyNotBound(essentials::StringId id) :
		exceptions::RuntimeError("Required property \"" + id.string() + "\" is not bound")
	{
	}

};

class PropertyObject;

class Properties {
public:

	Properties() = default;

	template <class T>
	Properties(T model) :
		object_(std::make_shared<Model<T>>(std::move(model)))
	{
	}

	PropertyObject get(essentials::StringId id) const {
		return object_->get(std::move(id));
	}

private:

	class Concept {
	public:

		virtual ~Concept() = default;

		virtual PropertyObject get(essentials::StringId id) const = 0;

	};

	template <class T>
	class Model : public Concept {
	public:

		Model(T model) :
			model_(std::move(model_))
		{
		}

		PropertyObject get(essentials::StringId id) const override {
			return getShaderProperty(model_, std::move(id));
		}

	private:

		T model_;

	};

	std::shared_ptr<const Concept> object_;

};

template <class T>
PropertyObject getShaderProperty(const T& model, essentials::StringId id) {
	return model.get(std::move(id));
}

template <class T>
PropertyObject getShaderProperty(essentials::observer_ptr<const T> model, essentials::StringId id) {
	return getShaderProperty(*model, std::move(id));
}

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PROPERTIES_HPP_ */
