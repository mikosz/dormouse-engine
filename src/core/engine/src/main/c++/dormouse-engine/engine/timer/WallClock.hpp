#ifndef _DORMOUSEENGINE_ENGINE_TIMER_WALLCLOCK_HPP_
#define _DORMOUSEENGINE_ENGINE_TIMER_WALLCLOCK_HPP_

#include <chrono>

namespace dormouse_engine::engine::timer {

class WallClock final {
public:

	using Seconds = float; // TODO: TEMP!

	static constexpr Seconds NO_LIMIT = -1.0f;

	WallClock() = default;

	WallClock(Seconds minFrameLength, Seconds maxFrameLength) :
		minFrameLength_(minFrameLength),
		maxFrameLength_(maxFrameLength)
	{
	}

	void tick() noexcept;

	Seconds lastFrameDuration() const noexcept {
		return lastFrameDuration_;
	}

private:

	using ChronoClock = std::chrono::high_resolution_clock;

	ChronoClock clock_;

	ChronoClock::time_point frameStart_ = ChronoClock::now();

	Seconds minFrameLength_ = NO_LIMIT;

	Seconds maxFrameLength_ = NO_LIMIT;

	Seconds lastFrameDuration_ = 0.0f;

};

} // namespace dormouse_engine::engine::timer

#endif /* _DORMOUSEENGINE_ENGINE_TIMER_WALLCLOCK_HPP_ */
