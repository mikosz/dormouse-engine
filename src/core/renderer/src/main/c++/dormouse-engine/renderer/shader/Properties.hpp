#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PROPERTIES_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PROPERTIES_HPP_

#include "dormouse-engine/exceptions/RuntimeError.hpp"
#include "dormouse-engine/essentials/StringId.hpp"

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

	const PropertyObject& get(essentials::StringId id) const {
		return object_->get(std::move(id));
	}

private:

	class Concept {
	public:

		virtual ~Concept() = default;

		virtual const Properties& get(essentials::StringId id) const = 0;

	};

	template <class T>
	class Model : public Concept {
	public:

		virtual const Properties& get(essentials::StringId id) const override {
			return shaderObjectProperty(model_, std::move(id));
		}

	private:

		T model_;

	};

	std::shared_ptr<const Concept> object_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PROPERTIES_HPP_ */
