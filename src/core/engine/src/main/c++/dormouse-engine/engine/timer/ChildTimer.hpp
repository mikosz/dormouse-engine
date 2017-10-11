#ifndef _DORMOUSEENGINE_ENGINE_TIMER_CHILDTIMER_HPP_
#define _DORMOUSEENGINE_ENGINE_TIMER_CHILDTIMER_HPP_

#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "Timer.hpp"

namespace dormouse_engine::engine::timer {

class ChildTimer final {
public:

	ChildTimer(essentials::observer_ptr<Timer> parent) :
		parent_(parent)
	{
	}

	Timer::Seconds lastFrameDuration() const noexcept {
		return parent_->lastFrameDuration() * multiplier_;
	}

	const float multiplier() const noexcept {
		return multiplier_;
	}

	float& multiplier() noexcept {
		return multiplier_;
	}

private:

	essentials::observer_ptr<Timer> parent_;

	float multiplier_ = 1.0f;

};

} // namespace dormouse_engine::engine::timer

#endif /* _DORMOUSEENGINE_ENGINE_TIMER_CHILDTIMER_HPP_ */
