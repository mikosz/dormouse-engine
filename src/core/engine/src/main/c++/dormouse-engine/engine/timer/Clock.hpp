#ifndef _DORMOUSEENGINE_ENGINE_TIMER_CLOCK_HPP_
#define _DORMOUSEENGINE_ENGINE_TIMER_CLOCK_HPP_

#include <chrono>

namespace dormouse_engine::engine::timer {

class Clock final {
public:

	using Seconds = float; // TODO: TEMP!

	void tick() noexcept {
		using namespace std::chrono_literals;
		using TimeResolution = std::chrono::microseconds;
		static constexpr auto TICKS_PER_SECOND =
			static_cast<float>(std::chrono::duration_cast<TimeResolution>(1s).count());

		const auto now = ChronoClock::now();
		
		lastFrameDuration_ =
			std::chrono::duration_cast<std::chrono::microseconds>(now - frameStart_).count() / TICKS_PER_SECOND;

		frameStart_ = now;
	}

	Seconds lastFrameDuration() const noexcept {
		return lastFrameDuration_;
	}

private:

	using ChronoClock = std::chrono::high_resolution_clock;

	ChronoClock clock_;

	ChronoClock::time_point frameStart_ = ChronoClock::now();

	Seconds lastFrameDuration_ = 0.0f;

};

} // namespace dormouse_engine::engine::timer

#endif /* _DORMOUSEENGINE_ENGINE_TIMER_CLOCK_HPP_ */
