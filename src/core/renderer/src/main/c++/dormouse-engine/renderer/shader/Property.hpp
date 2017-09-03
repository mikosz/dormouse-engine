#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_

#include <memory>
#include <cassert>

#include "dormouse-engine/graphics/ShaderType.hpp"
#include "../command/commandfwd.hpp"

namespace dormouse_engine::renderer::shader {

class ResourceProperty {
public:

	ResourceProperty() = default;

	template <class T>
	ResourceProperty(T model) :
		object_(std::make_shared<Model<T>>(std::move(model)))
	{
	}

	void bind(command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot) const {
		assert(static_cast<bool>(object_));
		object_->bind(cmd, stage, slot);
	}

private:

	class Concept {
	public:

		virtual ~Concept() = default;

		virtual void bind(command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot) const = 0;

	};

	template <class T>
	class Model : public Concept {
	public:

		void bind(command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot) const override {
			assert(!"unimplemented");
		}

	private:

		T model_;

	};

	std::shared_ptr<const Concept> object_;

};

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_ */
