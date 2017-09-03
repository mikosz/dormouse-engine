#ifndef _DORMOUSEENGINE_REFLECTION_OBJECT_HPP_
#define _DORMOUSEENGINE_REFLECTION_OBJECT_HPP_

#include <memory>
#include <cassert>

namespace dormouse_engine::reflection {

class Object {
public:

	template <class T>
	Object(T model) :
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

} // namespace dormouse_engine::reflection

#endif /* _DORMOUSEENGINE_REFLECTION_OBJECT_HPP_ */
