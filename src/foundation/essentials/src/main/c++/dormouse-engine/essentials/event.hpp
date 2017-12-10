#ifndef _DORMOUSEENGINE_ESSENTIALS_EVENT_HPP_
#define _DORMOUSEENGINE_ESSENTIALS_EVENT_HPP_

#include <variant>
#include <memory>
#include <vector>
#include <functional>

#include "raii-helper.hpp"
#include "Null.hpp"

namespace dormouse_engine::essentials {

template <class... EventTypes>
class EventBroadcaster final {
public:

	using ListenerRegistration = RaiiHelper;

	template <class EventType, class CallbackFunctionType>
	[[nodiscard]] ListenerRegistration subscribe(CallbackFunctionType f) {
		auto index = size_t();

		if (free_.empty()) {
			listeners_.emplace_back(CallbackType<EventType>(std::move(f)));
			index = listeners_.size() - 1;
		} else {
			index = free_.back();
			listeners_[free_.back()] = CallbackType<EventType>(std::move(f));
			free_.pop_back();
		}

		return ListenerRegistration([this, index]() {
				listeners_[index].reset();
				free_.emplace_back(index);
			});
	}

	template <class EventType>
	void notify(const EventType& event) const {
		for (const auto& listener : listeners_) {
			listener.notify(event);
		}
	}

private:

	template <class EventType>
	using CallbackType = std::function<void (const EventType&)>;

	template <class... EventTypes>
	class EventListener final {
	public:

		EventListener() = default;

		template <class EventType>
		EventListener(CallbackType<EventType> callback) :
			callback_(std::move(callback))
		{
		}

		void reset() {
			callback_ = Null::null;
		}

		template <class EventType>
		void notify(const EventType& event) const {
			const auto* callbackPtr = std::get_if<CallbackType<EventType>>(&callback_);
			if (callbackPtr != nullptr) {
				(*callbackPtr)(event);
			}
		}

	private:

		std::variant<Null, CallbackType<EventTypes>...> callback_;

	};

	using Free = std::vector<size_t>;

	using Listener = EventListener<EventTypes...>;

	using Listeners = std::vector<Listener>;

	Free free_;

	Listeners listeners_;

};

} // namespace dormouse_engine::essentials

namespace dormouse_engine {

using essentials::EventBroadcaster;

} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_ESSENTIALS_EVENT_HPP_ */
