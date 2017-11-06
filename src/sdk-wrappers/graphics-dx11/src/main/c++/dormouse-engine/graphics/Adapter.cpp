#include "graphics.pch.hpp"

#include "Adapter.hpp"

#include "DirectXError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

namespace /* anonymous */ {

system::windows::COMWrapper<IDXGIFactory> createDXGIFactory() {
	system::windows::COMWrapper<IDXGIFactory> factory;
	checkDirectXCall(
		CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory.get())),
		"Failed to create a DXGIFactory"
	);

	return factory;
}

} // anonymous namespace

std::vector<Adapter> Adapter::create() {
	auto adapters = std::vector<Adapter>();

	auto dxgiFactory = createDXGIFactory();

	for (;;) {
		auto adapter = system::windows::COMWrapper<IDXGIAdapter>();
		auto result = dxgiFactory->EnumAdapters(0, &adapter.get());

		if (result == DXGI_ERROR_NOT_FOUND) {
			break;
		} else {
			checkDirectXCall(result, "Failed to enumerate video cards");
			adapters.emplace_back(Adapter(std::move(adapter)));
		}
	}

	return adapters;
}
