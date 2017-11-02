#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/renderer/control/ResourceView.hpp"
#include "dormouse-engine/tester/App.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::control;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(RendererResourceViewTestSuite);

BOOST_AUTO_TEST_CASE(ReturnsEqualResourceViewsForSameTexture1d) {
	auto& app = *tester::GlobalApp::instance();

	auto configuration = graphics::Texture::Configuration1d();
	configuration.allowCPUWrite = true;
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = true;
	configuration.pixelFormat = graphics::FORMAT_B8G8R8X8_UNORM;
	configuration.purposeFlags = graphics::Texture::Purpose::SHADER_RESOURCE;
	configuration.width = 10u;

	const auto texture = graphics::Texture(app.graphicsDevice(), configuration);

	auto resourceViewL = ResourceView(texture);
	auto resourceViewR = ResourceView(texture);

	BOOST_CHECK(resourceViewL == resourceViewR);
}

BOOST_AUTO_TEST_CASE(ReturnsUnequalResourceViewsForDifferentTexture1d) {
	auto& app = *tester::GlobalApp::instance();

	auto configuration = graphics::Texture::Configuration1d();
	configuration.allowCPUWrite = true;
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = true;
	configuration.pixelFormat = graphics::FORMAT_B8G8R8X8_UNORM;
	configuration.purposeFlags = graphics::Texture::Purpose::SHADER_RESOURCE;
	configuration.width = 10u;

	const auto textureL = graphics::Texture(app.graphicsDevice(), configuration);
	const auto textureR = graphics::Texture(app.graphicsDevice(), configuration);

	auto resourceViewL = ResourceView(textureL);
	auto resourceViewR = ResourceView(textureR);

	BOOST_CHECK(resourceViewL != resourceViewR);
}

BOOST_AUTO_TEST_CASE(ReturnsEqualResourceViewsForSameTexture2d) {
	auto& app = *tester::GlobalApp::instance();

	auto configuration = graphics::Texture::Configuration2d();
	configuration.allowCPUWrite = true;
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = true;
	configuration.pixelFormat = graphics::FORMAT_B8G8R8X8_UNORM;
	configuration.purposeFlags = graphics::Texture::Purpose::SHADER_RESOURCE;
	configuration.width = 10u;
	configuration.height = 10u;

	const auto texture = graphics::Texture(app.graphicsDevice(), configuration);

	auto resourceViewL = ResourceView(texture);
	auto resourceViewR = ResourceView(texture);

	BOOST_CHECK(resourceViewL == resourceViewR);
}

BOOST_AUTO_TEST_CASE(ReturnsUnequalResourceViewsForDifferentTexture2d) {
	auto& app = *tester::GlobalApp::instance();

	auto configuration = graphics::Texture::Configuration2d();
	configuration.allowCPUWrite = true;
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = true;
	configuration.pixelFormat = graphics::FORMAT_B8G8R8X8_UNORM;
	configuration.purposeFlags = graphics::Texture::Purpose::SHADER_RESOURCE;
	configuration.width = 10u;
	configuration.height = 10u;

	const auto textureL = graphics::Texture(app.graphicsDevice(), configuration);
	const auto textureR = graphics::Texture(app.graphicsDevice(), configuration);

	auto resourceViewL = ResourceView(textureL);
	auto resourceViewR = ResourceView(textureR);

	BOOST_CHECK(resourceViewL != resourceViewR);
}

BOOST_AUTO_TEST_CASE(ReturnsEqualResourceViewsForSameBuffer) {
	auto& app = *tester::GlobalApp::instance();

	auto configuration = graphics::Buffer::Configuration();
	configuration.allowCPUWrite = true;
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = true;
	configuration.size = 10u;
	configuration.purpose = graphics::Buffer::Purpose::SHADER_RESOURCE;

	const auto buffer = graphics::Buffer(app.graphicsDevice(), configuration);

	auto resourceViewL = ResourceView(buffer, graphics::FORMAT_B8G8R8A8_UNORM);
	auto resourceViewR = ResourceView(buffer, graphics::FORMAT_B8G8R8A8_UNORM);

	BOOST_CHECK(resourceViewL == resourceViewR);
}

BOOST_AUTO_TEST_CASE(ReturnsUnequalResourceViewsForDifferentBuffer) {
	auto& app = *tester::GlobalApp::instance();

	auto configuration = graphics::Buffer::Configuration();
	configuration.allowCPUWrite = true;
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = true;
	configuration.size = 36u;
	configuration.purpose = graphics::Buffer::Purpose::SHADER_RESOURCE;

	const auto bufferL = graphics::Buffer(app.graphicsDevice(), configuration);
	const auto bufferR = graphics::Buffer(app.graphicsDevice(), configuration);

	auto resourceViewL = ResourceView(bufferL, graphics::FORMAT_R32G32B32_FLOAT);
	auto resourceViewR = ResourceView(bufferR, graphics::FORMAT_R32G32B32_FLOAT);

	BOOST_CHECK(resourceViewL != resourceViewR);
}

BOOST_AUTO_TEST_CASE(ReturnsUnequalResourceViewsForSameBufferWithDifferentElementFormat) {
	auto& app = *tester::GlobalApp::instance();

	auto configuration = graphics::Buffer::Configuration();
	configuration.allowCPUWrite = true;
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = true;
	configuration.size = 12u;
	configuration.purpose = graphics::Buffer::Purpose::SHADER_RESOURCE;

	const auto buffer = graphics::Buffer(app.graphicsDevice(), configuration);

	auto resourceViewL = ResourceView(buffer, graphics::FORMAT_R32_FLOAT);
	auto resourceViewR = ResourceView(buffer, graphics::FORMAT_R32_UINT);

	BOOST_CHECK(resourceViewL != resourceViewR);
}

BOOST_AUTO_TEST_SUITE_END(/* RendererResourceViewTestSuite */);

} // anonymous namespace
