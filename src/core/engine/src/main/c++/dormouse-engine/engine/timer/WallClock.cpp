#include "WallClock.hpp"

#include <thread>

using namespace dormouse_engine::engine::timer;

void WallClock::tick() noexcept {
	using namespace std::chrono_literals;
	using TimeResolution = std::chrono::microseconds;
	static constexpr auto TICKS_PER_SECOND =
		static_cast<float>(std::chrono::duration_cast<TimeResolution>(1s).count());

	auto now = ChronoClock::now();

	lastFrameDuration_ =
		std::chrono::duration_cast<std::chrono::microseconds>(now - frameStart_).count() / TICKS_PER_SECOND;

	if (maxFrameLength_ > 0.0f && lastFrameDuration_ > maxFrameLength_) {
		lastFrameDuration_ = maxFrameLength_;
	} else if (minFrameLength_ > 0.0f && lastFrameDuration_ < minFrameLength_) {
		const auto sleepFor =
			std::chrono::microseconds(static_cast<int>((minFrameLength_ - lastFrameDuration_) * TICKS_PER_SECOND));
		std::this_thread::sleep_for(sleepFor);
		now += sleepFor; // TODO: not checking for how long we slept, ok?
		lastFrameDuration_ = minFrameLength_;
	}

	frameStart_ = now;
}
