#ifndef _DORMOUSEENGINE_RENDERER_COMMAND_COMMAND_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMAND_COMMAND_HPP_

#include <memory>

#include "dormouse-engine/graphics/CommandList.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/essentials/types.hpp"
#include "CommandKey.hpp"

namespace dormouse_engine::renderer::command {

class Command final {
public:

	template <class T>
	Command(essentials::observer_ptr<T> model) :
		self_(std::move(model))
	{
	}
	
	CommandKey key() const {
		return self_->key();
	}

	void submit(graphics::CommandList& commandList, const Command* previous = nullptr) const {
		self_->submit(commandList, previous ? previous->self_.get() : nullptr);
	}

private:

	class Concept {
	public:

		virtual ~Concept() = default;

		virtual CommandKey key() const = 0;

		virtual void submit(graphics::CommandList& commandList, const Concept* previous) const = 0;

	};

	template <class T>
	class Model final : public Concept {
	public:

		Model(T object) :
			object_(std::move(object))
		{
		}

		CommandKey key() const override {
			return object_.key();
		}

		void submit(graphics::CommandList& commandList, const Concept* previous) const override {
			const auto* previousModel = dynamic_cast<const Model*>(previous);
			essentials::dereference(object_).submit(
				commandList,
				previousModel ? &previousModel->object_ : nullptr
				);
		}

	private:

		essentials::observer_ptr<T> object_;

	};

	std::shared_ptr<const Concept> self_;

};

} // namespace dormouse_engine::renderer::command

#endif /* _DORMOUSEENGINE_RENDERER_COMMAND_COMMAND_HPP_ */
