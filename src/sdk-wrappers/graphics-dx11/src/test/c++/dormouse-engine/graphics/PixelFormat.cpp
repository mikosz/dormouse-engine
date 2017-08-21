#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/graphics/PixelFormat.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(GraphicsPixelFormatTestSuite);

BOOST_AUTO_TEST_CASE(PixelFormatInitialisesFromChannels) {
	constexpr auto format = R32_FLOAT << G32_FLOAT;
	static_assert(format.channel(0).type == PixelFormat::ChannelType::RED);
	static_assert(format.channel(1).type == PixelFormat::ChannelType::GREEN);
	static_assert(format.channelsUsed() == 2);

	BOOST_CHECK_THROW(format.channel(2), exceptions::LogicError);
}

BOOST_AUTO_TEST_CASE(CalculatesPixelSizeProperly) {
	constexpr auto format = FORMAT_R16_UINT;
	static_assert(format.pixelSize() == 2u);

	constexpr auto bcFormat = FORMAT_BC1_UNORM;
	const auto i = bcFormat.pixelSize();
	static_assert(bcFormat.pixelSize() == 1u);
}

BOOST_AUTO_TEST_CASE(CalculatesRowPitchProperly) {
	constexpr auto format = FORMAT_R32G32_FLOAT;
	static_assert(format.rowPitch(10u) == 8u * 10u);

	constexpr auto bcFormat = FORMAT_BC1_UNORM;
	static_assert(bcFormat.rowPitch(10u) == 1u * 3u);
}

BOOST_AUTO_TEST_CASE(CalculatesSlicePitchProperly) {
	constexpr auto format = FORMAT_R32G32_FLOAT;
	static_assert(format.slicePitch(10u, 8u * 10u) == 8u * 10u * 10u);

	constexpr auto bcFormat = FORMAT_BC1_UNORM;
	static_assert(bcFormat.slicePitch(8u, 1u * 3u) == 1u * 3u * 2u);
}

BOOST_AUTO_TEST_SUITE_END(/* GraphicsPixelFormatTestSuite */);

} // anonymous namespace
