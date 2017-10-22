#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/essentials/test-utils/GMockFixture.hpp"
#include "dormouse-engine/essentials/event.hpp"

using namespace dormouse_engine::essentials;

namespace /* anonymous */ {

struct Listener {

	MOCK_CONST_METHOD1(notify, void (int));

};

BOOST_FIXTURE_TEST_SUITE(DormouseEngineEssentialsEventTestSuite, test_utils::GMockFixture);

BOOST_AUTO_TEST_CASE(BroadcasterNotifiesRegisteredListeners) {
	auto broadcaster = EventBroadcaster<int>();

	Listener one{};
	Listener two{};

	EXPECT_CALL(one, notify(42));
	EXPECT_CALL(two, notify(42));

	auto registrarOne = broadcaster.subscribe([&one](int i) { one.notify(i); });
	auto registrarTwo = broadcaster.subscribe([&two](int i) { two.notify(i); });

	broadcaster.notify(42);
}

BOOST_AUTO_TEST_CASE(BroadcasterDoesntNotifyUnregisteredListeners) {
	auto broadcaster = EventBroadcaster<int>();

	Listener one{};
	Listener two{};
	Listener three{};

	EXPECT_CALL(one, notify(42)).Times(2);
	EXPECT_CALL(two, notify(42)).Times(1);
	EXPECT_CALL(three, notify(42)).Times(1);

	{
		auto registrarOne = broadcaster.subscribe([&one](int i) { one.notify(i); });

		{
			auto registrarTwo = broadcaster.subscribe([&two](int i) { two.notify(i); });
			broadcaster.notify(42);
		}

		broadcaster.notify(42);
	}

	{
		auto registrarThree = broadcaster.subscribe([&three](int i) { three.notify(i); });
		broadcaster.notify(42);
	}

	broadcaster.notify(42);
}

BOOST_AUTO_TEST_SUITE_END(/* DormouseEngineEssentialsEventTestSuite */);

} // anonymous namespace
