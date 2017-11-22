#include "graphics.pch.hpp"

#include "Adapter.hpp"

#include "detail/Internals.hpp"
#include "DirectXError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

std::vector<DisplayMode> Output::displayModes(PixelFormat pixelFormat) const {
	const auto dxgiFormat = static_cast<DXGI_FORMAT>(pixelFormat.id());

	auto displayModes = std::vector<DisplayMode>();
	auto modeCount = UINT();
	auto displayModeListResult = HRESULT();
	displayModeListResult = output_->GetDisplayModeList(
		dxgiFormat, DXGI_ENUM_MODES_INTERLACED, &modeCount, nullptr);

	if (displayModeListResult != DXGI_ERROR_NOT_CURRENTLY_AVAILABLE) {
		checkDirectXCall(displayModeListResult, "Failed to get display mode count");

		displayModes.reserve(modeCount);

		auto dxgiDisplayModes = std::vector<DXGI_MODE_DESC>();
		dxgiDisplayModes.resize(modeCount);
		checkDirectXCall(
			output_->GetDisplayModeList(
				dxgiFormat, DXGI_ENUM_MODES_INTERLACED, &modeCount, dxgiDisplayModes.data()),
			"Failed to get display modes"
			);

		for (const auto& dxgiDisplayMode : dxgiDisplayModes) {
			auto& displayMode = displayModes.emplace_back();
			displayMode.width = dxgiDisplayMode.Width;
			displayMode.height = dxgiDisplayMode.Height;
			displayMode.refreshRateNumerator = dxgiDisplayMode.RefreshRate.Numerator;
			displayMode.refreshRateDenominator = dxgiDisplayMode.RefreshRate.Denominator;
			displayMode.pixelFormat = pixelFormat;
		}
	}

	return displayModes;
}

std::vector<Adapter> Adapter::create() {
	auto adapters = std::vector<Adapter>();

	auto dxgiFactory = detail::Internals::createDXGIFactory();

	auto adapterIdx = UINT(0);
	for (;;) {
		auto adapter = system::windows::COMWrapper<IDXGIAdapter>();
		auto result = dxgiFactory->EnumAdapters(adapterIdx, &adapter.get());

		if (result == DXGI_ERROR_NOT_FOUND) {
			break;
		} else {
			checkDirectXCall(result, "Failed to enumerate video cards");
			adapters.emplace_back(Adapter(std::move(adapter)));
			++adapterIdx;
		}
	}

	return adapters;
}

std::vector<Output> Adapter::outputs() const {
	auto outputs = std::vector<Output>();

	auto outputIdx = UINT(0);
	for (;;) {
		auto output = system::windows::COMWrapper<IDXGIOutput>();
		const auto result = adapter_->EnumOutputs(outputIdx, &output.get());

		if (result == DXGI_ERROR_NOT_FOUND) {
			break;
		} else {
			checkDirectXCall(result, "Failed to enumerate outputs");
			outputs.push_back(std::move(output));
			++outputIdx;
		}
	}

	return outputs;
}
