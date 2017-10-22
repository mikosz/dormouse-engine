#ifndef _DORMOUSEENGINE_ESSENTIALS_EVENT_HPP_
#define _DORMOUSEENGINE_ESSENTIALS_EVENT_HPP_

#include <memory>
#include <vector>
#include <functional>

#include "raii-helper.hpp"

namespace dormouse_engine::essentials {

template <class... Args>
class EventListener final {
public:

	using Callback = std::function<void (Args&&...)>;

	EventListener() = default;

	EventListener(Callback listener) :
		callback_(std::move(listener))
	{
	}

	void reset() {
		callback_ = Callback();
	}

	void notify(Args&&... args) const {
		if (callback_) {
			callback_(std::forward<Args>(args)...);
		}
	}

private:

	Callback callback_;

};

template <class... ListenerArgs>
class EventBroadcaster final {
public:

	using Listener = EventListener<ListenerArgs...>;

	using ListenerRegistrar = RaiiHelper;

	[[nodiscard]] ListenerRegistrar subscribe(Listener listener) {
		auto index = size_t();

		if (free_.empty()) {
			listeners_.emplace_back(std::move(listener));
			index = listeners_.size() - 1;
		} else {
			index = free_.back();
			listeners_[free_.back()] = std::move(listener);
			free_.pop_back();
		}

		return ListenerRegistrar([this, index]() {
				listeners_[index].reset();
				free_.emplace_back(index);
			});
	}

	[[nodiscard]] ListenerRegistrar subscribe(typename Listener::Callback callback) {
		return subscribe(Listener(std::move(callback)));
	}

	void notify(ListenerArgs&&... args) const {
		for (const auto& listener : listeners_) {
			listener.notify(std::forward<ListenerArgs>(args)...);
		}
	}

private:

	using Free = std::vector<size_t>;

	using Listeners = std::vector<Listener>;

	Free free_;

	Listeners listeners_;

};

} // namespace dormouse_engine::essentials

namespace dormouse_engine {

using essentials::EventListener;
using essentials::EventBroadcaster;

} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_ESSENTIALS_EVENT_HPP_ */
