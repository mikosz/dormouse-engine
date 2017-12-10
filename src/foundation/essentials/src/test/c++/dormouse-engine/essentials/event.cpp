#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/essentials/test-utils/GMockFixture.hpp"
#include "dormouse-engine/essentials/event.hpp"

using namespace dormouse_engine::essentials;

namespace /* anonymous */ {

struct S {
};

struct Listener {

	MOCK_CONST_METHOD0(notifyVoid, void ());

	MOCK_CONST_METHOD0(otherNotifyVoid, void ());

	MOCK_CONST_METHOD1(notifyWithInt, void (int));

};

struct VoidEvent {
};

struct OtherVoidEvent {
};

struct IntEvent {
	int i;
};

BOOST_FIXTURE_TEST_SUITE(DormouseEngineEssentialsEventTestSuite, test_utils::GMockFixture);

BOOST_AUTO_TEST_CASE(BroadcasterNotifiesRegisteredListeners) {
	auto broadcaster = EventBroadcaster<VoidEvent>();

	Listener one{};
	Listener two{};

	EXPECT_CALL(one, notifyVoid());
	EXPECT_CALL(two, notifyVoid());

	auto registrarOne = broadcaster.subscribe<VoidEvent>([&one](const VoidEvent&) { one.notifyVoid(); });
	auto registrarTwo = broadcaster.subscribe<VoidEvent>([&two](const VoidEvent&) { two.notifyVoid(); });

	broadcaster.notify(VoidEvent{});
}

BOOST_AUTO_TEST_CASE(BroadcasterNotifiesOnlyListenersForSpecificEvent) {
	auto broadcaster = EventBroadcaster<VoidEvent, OtherVoidEvent>();

	Listener one{};
	Listener two{};

	EXPECT_CALL(two, otherNotifyVoid());

	auto registrarOne = broadcaster.subscribe<VoidEvent>([&one](const VoidEvent&) { one.notifyVoid(); });
	auto registrarTwo =
		broadcaster.subscribe<OtherVoidEvent>([&two](const OtherVoidEvent&) { two.otherNotifyVoid(); });

	broadcaster.notify(OtherVoidEvent{});
}

BOOST_AUTO_TEST_CASE(BroadcasterPassesEventDataToListeners) {
	auto broadcaster = EventBroadcaster<IntEvent>();

	Listener one{};

	EXPECT_CALL(one, notifyWithInt(42));

	auto registrarOne = broadcaster.subscribe<IntEvent>([&one](const IntEvent& e) { one.notifyWithInt(e.i); });

	broadcaster.notify(IntEvent{ 42 });
}

BOOST_AUTO_TEST_CASE(BroadcasterDoesntNotifyUnregisteredListeners) {
	auto broadcaster = EventBroadcaster<VoidEvent>();

	Listener one{};
	Listener two{};
	Listener three{};

	EXPECT_CALL(one, notifyVoid()).Times(2);
	EXPECT_CALL(two, notifyVoid()).Times(1);
	EXPECT_CALL(three, notifyVoid()).Times(1);

	{
		auto registrarOne = broadcaster.subscribe<VoidEvent>([&one](const VoidEvent&) { one.notifyVoid(); });

		{
			auto registrarTwo =
				broadcaster.subscribe<VoidEvent>([&two](const VoidEvent&) { two.notifyVoid(); });
			broadcaster.notify(VoidEvent{});
		}

		broadcaster.notify(VoidEvent{});
	}

	{
		auto registrarThree =
			broadcaster.subscribe<VoidEvent>([&three](const VoidEvent&) { three.notifyVoid(); });
		broadcaster.notify(VoidEvent{});
	}

	broadcaster.notify(VoidEvent{});
}

BOOST_AUTO_TEST_SUITE_END(/* DormouseEngineEssentialsEventTestSuite */);

} // anonymous namespace
