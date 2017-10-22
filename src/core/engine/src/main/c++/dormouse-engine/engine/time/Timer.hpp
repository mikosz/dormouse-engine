#ifndef _DORMOUSEENGINE_ENGINE_TIME_TIMER_HPP_
#define _DORMOUSEENGINE_ENGINE_TIME_TIMER_HPP_

#include <memory>

#include "dormouse-engine/essentials/memory.hpp"

namespace dormouse_engine::engine::time {

class Timer final { // TODO: rename to TimerPtr? TimerRef?
public:

	using Seconds = float; // TODO: TEMP!

	template <class T>
	Timer(essentials::observer_ptr<T> timerModel) :
		storage_(std::make_shared<Model<T>>(std::move(timerModel)))
	{
	}

	Seconds lastFrameDuration() const noexcept {
		return storage_->lastFrameDuration();
	}

private:

	class Concept {
	public:

		virtual ~Concept() = default;

		virtual Seconds lastFrameDuration() const noexcept = 0;

	};

	template <class T>
	class Model : public Concept {
	public:

		Model(essentials::observer_ptr<const T> model) :
			model_(std::move(model))
		{
		}

		Seconds lastFrameDuration() const noexcept override {
			return model_->lastFrameDuration();
		}

	private:

		essentials::observer_ptr<const T> model_;

	};

	std::shared_ptr<const Concept> storage_;

};

} // namespace dormouse_engine::engine::time

#endif /* _DORMOUSEENGINE_ENGINE_TIME_TIMER_HPP_ */
