#ifndef _DORMOUSEENGINE_EVENT_EVENT_HPP_
#define _DORMOUSEENGINE_EVENT_EVENT_HPP_

#include <any>
#include <vector>
#include <unordered_map>

namespace dormouse_engine::event {

template <class EventTraitsT, class EventHandlerTraitsT>
class Broadcaster {
public:

	using EventId = typename EventTraitsT::EventId;

	using EventHandlerTraits = EventHandlerTraitsT;

	template <class EventType, class FunctionT>
	EventHandlerRegistration register(typename EventHandlerTraits<EventType>::HandlerType handler);

	template <class... Args>
	void notify(EventId eventId, Args&& args...) const {
		auto range = handlers_.equal_range(eventId);
		while (range.first != range.second) {
			const auto& handler = *range.first;
			std::any_cast<EventTraitsT::EventHandlerType<>>(handler.callback)
			++range.first;
		}
	}

private:

	struct EventHandler {
		using EventHandlerId = std::intptr_t;
		EventHandlerId id;
		std::any callback;
	};

	std::unordered_multimap<EventId, EventHandler> handlers_;

	void unregister();

	friend class EventHandlerRegistration;

};

} // namespace dormouse_engine::event

#endif /* _DORMOUSEENGINE_EVENT_EVENT_HPP_ */
