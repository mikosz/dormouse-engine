#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_

#include <memory>
#include <type_traits>
#include <typeinfo>

#include "dormouse-engine/exceptions/RuntimeError.hpp"
#include "dormouse-engine/essentials/StringId.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/graphics/ShaderType.hpp"
#include "../control/ResourceView.hpp"

namespace dormouse_engine::renderer::shader {

class PropertyNotBound : public exceptions::RuntimeError {
public:

	PropertyNotBound(essentials::StringId id) :
		exceptions::RuntimeError("Required property \"" + id.string() + "\" is not bound")
	{
	}

};

class InvalidTargetType : public exceptions::RuntimeError {
public:

	InvalidTargetType(const std::string& type_name) :
		exceptions::RuntimeError("Can't cast property to " + type_name)
	{
	}

};

class Property {
public:

	Property() = default;

	template <class T>
	Property(T model) :
	{
		new(&object_) Model<T>(std::move(model));
	}

	~Property() {
		reinterpret_cast<Concept*>(&object_)->~Concept();
	}

	Property get(essentials::StringId id) const {
		return reinterpret_cast<const Concept*>(&object_)->get(std::move(id));
	}

	template <class T>
	const T& as() const {
		const auto* concept = reinterpret_cast<const Concept*>(&object_);
		const auto* object = reinterpret_cast<const Model<T>*>(concept);
		
		if (object == nullptr) {
			throw InvalidTargetType(typeid(T).name());
		}
		
		return object->model();
	}

private:

	class Concept {
	public:

		virtual ~Concept() = default;

		virtual Property get(essentials::StringId id) const = 0;

	};

	template <class T>
	class Model : public Concept {
	public:

		T may only be a fundamental type or pointer, enforce this

		Model(T model) :
			model_(std::move(model))
		{
		}

		Property get(essentials::StringId id) const override {
			return getShaderProperty(model_, std::move(id));
		}

		const T& model() const {
			return model_;
		}

	private:

		T model_;

	};

	std::aligned_storage_t<sizeof(Model<void*>), alignof(Model<void*>)> object_;

};

template <class T>
Property getShaderProperty(const T& model, essentials::StringId id);

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_ */
