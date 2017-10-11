#ifndef _DORMOUSEENGINE_ENGINE_TIMER_TIMER_HPP_
#define _DORMOUSEENGINE_ENGINE_TIMER_TIMER_HPP_

#include "dormouse-engine/essentials/PolymorphicStorage.hpp"
#include "dormouse-engine/essentials/types.hpp"

namespace dormouse_engine::engine::timer {

class Timer final {
public:

	using Seconds = float; // TODO: TEMP!

	template <class T>
	Timer(T timerModel) :
		storage_(std::move(timerModel))
	{
	}

	Seconds lastFrameDuration() const noexcept {
		return storage_->lastFrameDuration();
	}

private:

	class Concept : public essentials::ConceptBase {
	public:

		virtual ~Concept() = default;

		virtual Seconds lastFrameDuration() const noexcept = 0;

	};

	template <class T>
	class Model : public essentials::ModelBase<Concept, Model<T>, T> {
	public:

		Seconds lastFrameDuration() const noexcept override {
			return essentials::dereference(model_).lastFrameDuration();
		}

	};

	essentials::PolymorphicStorage<Concept, Model, sizeof(float) + sizeof(void*), alignof(void*)> storage_;

};

} // namespace dormouse_engine::engine::timer

#endif /* _DORMOUSEENGINE_ENGINE_TIMER_TIMER_HPP_ */
