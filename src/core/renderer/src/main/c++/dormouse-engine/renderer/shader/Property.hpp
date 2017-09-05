#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_

#include <memory>
#include <cassert>

#include "dormouse-engine/graphics/ShaderType.hpp"
#include "../command/commandfwd.hpp"

namespace dormouse_engine::renderer::shader {

class Properties;

class Property {
public:

	Property() = default;

	template <class T>
	Property(T model) :
		object_(std::make_shared<Model<T>>(std::move(model)))
	{
	}

	const Properties& properties() const {
		object_->properties();
	}

private:

	class Concept {
	public:

		virtual ~Concept() = default;

		virtual const Properties& properties() const = 0;

	};

	template <class T>
	class Model : public Concept {
	public:

		virtual const Properties& properties() const override {
			return shaderObjectProperties(model_);
		}

	private:

		T model_;

	};

	std::shared_ptr<const Concept> object_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_ */
