#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_PIXELFORMAT_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_PIXELFORMAT_HPP_

#include <array>
#include <cstdint>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/exceptions/LogicError.hpp"
#include "dormouse-engine/essentials/types.hpp"
#include "dormouse-engine/enums.hpp"

namespace dormouse_engine::graphics {

DE_ENUM_VALUES(
	PixelFormatId,
	(R32_FLOAT)(DXGI_FORMAT_R32_FLOAT)
	(R32G32_FLOAT)(DXGI_FORMAT_R32G32_FLOAT)
	(R32G32B32_FLOAT)(DXGI_FORMAT_R32G32B32_FLOAT)
	(R32G32B32A32_FLOAT)(DXGI_FORMAT_R32G32B32A32_FLOAT)

	(R32_UINT)(DXGI_FORMAT_R32_UINT)

	(R8G8B8A8_UNORM)(DXGI_FORMAT_R8G8B8A8_UNORM)
	(B8G8R8A8_UNORM)(DXGI_FORMAT_B8G8R8A8_UNORM)
	(B8G8R8X8_UNORM)(DXGI_FORMAT_B8G8R8X8_UNORM)

	(R8G8B8A8_UNORM_SRGB)(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)

	(D32_FLOAT)(DXGI_FORMAT_D32_FLOAT)
);

class PixelFormat {
public:

	DE_MEMBER_ENUM(
		ChannelType,
		(UNUSED)
		(RESERVED)

		(RED)
		(GREEN)
		(BLUE)
		(ALPHA)

		(COMPRESSION_BLOCK)

		(DEPTH)
	);

	DE_MEMBER_ENUM(
		DataType,
		(UNKNOWN)
		(TYPELESS)
		(FLOAT)
		(UINT)
		(SINT)
		(UNORM)
		(SNORM)
		(UNORM_SRGB)
	);

	struct Channel {
		ChannelType type = ChannelType::UNUSED;
		DataType dataType = DataType::UNKNOWN;
		std::uint8_t size = 0u;

		constexpr Channel() = default;

		constexpr Channel(ChannelType type, DataType dataType, std::uint8_t size) :
			type(type),
			dataType(dataType),
			size(size)
		{
		}

	};

	constexpr PixelFormat() = default;

	constexpr PixelFormat(PixelFormatId pixelFormatId);

	constexpr PixelFormat(const PixelFormat& other, Channel next) :
		channels_(other.channels_)
	{
		for (size_t i = 0; i < PixelFormat::MAX_CHANNELS; ++i) {
			if (channels_[i].type == ChannelType::UNUSED) {
				channels_[i] = std::move(next);
				return;
			}
		}

		throw exceptions::LogicError("Couldn't find an unused channel");
	}

	constexpr size_t pixelSize() const;

	constexpr const Channel& channel(size_t index) const {
		if (index >= MAX_CHANNELS) {
			throw exceptions::LogicError("Invalid channel index");
		}

		return channels_[index];
	}

private:

	static const size_t MAX_CHANNELS = 4u;

	std::array<Channel, MAX_CHANNELS> channels_;

};

constexpr PixelFormat operator<<(PixelFormat lhs, PixelFormat::Channel rhs) {
	return PixelFormat(lhs, rhs);
}

namespace detail {

// --- channels

constexpr auto R32_FLOAT = PixelFormat::Channel(PixelFormat::ChannelType::RED, PixelFormat::DataType::FLOAT, 32u);
constexpr auto G32_FLOAT = PixelFormat::Channel(PixelFormat::ChannelType::GREEN, PixelFormat::DataType::FLOAT, 32u);
constexpr auto B32_FLOAT = PixelFormat::Channel(PixelFormat::ChannelType::BLUE, PixelFormat::DataType::FLOAT, 32u);
constexpr auto A32_FLOAT = PixelFormat::Channel(PixelFormat::ChannelType::ALPHA, PixelFormat::DataType::FLOAT, 32u);

constexpr auto R32_UINT = PixelFormat::Channel(PixelFormat::ChannelType::RED, PixelFormat::DataType::UINT, 32u);

constexpr auto R8_UNORM_SRGB = PixelFormat::Channel(PixelFormat::ChannelType::RED, PixelFormat::DataType::UNORM_SRGB, 8u);
constexpr auto G8_UNORM_SRGB = PixelFormat::Channel(PixelFormat::ChannelType::GREEN, PixelFormat::DataType::UNORM_SRGB, 8u);
constexpr auto B8_UNORM_SRGB = PixelFormat::Channel(PixelFormat::ChannelType::BLUE, PixelFormat::DataType::UNORM_SRGB, 8u);
constexpr auto A8_UNORM_SRGB = PixelFormat::Channel(PixelFormat::ChannelType::ALPHA, PixelFormat::DataType::UNORM_SRGB, 8u);

constexpr auto R8_UNORM = PixelFormat::Channel(PixelFormat::ChannelType::RED, PixelFormat::DataType::UNORM, 8u);
constexpr auto G8_UNORM = PixelFormat::Channel(PixelFormat::ChannelType::GREEN, PixelFormat::DataType::UNORM, 8u);
constexpr auto B8_UNORM = PixelFormat::Channel(PixelFormat::ChannelType::BLUE, PixelFormat::DataType::UNORM, 8u);
constexpr auto A8_UNORM = PixelFormat::Channel(PixelFormat::ChannelType::ALPHA, PixelFormat::DataType::UNORM, 8u);
constexpr auto X8_UNORM = PixelFormat::Channel(PixelFormat::ChannelType::RESERVED, PixelFormat::DataType::UNORM, 8u);

constexpr auto BC1_UNORM = PixelFormat::Channel(PixelFormat::ChannelType::COMPRESSION_BLOCK, PixelFormat::DataType::UNORM, 0u);

constexpr auto D32_FLOAT = PixelFormat::Channel(PixelFormat::ChannelType::DEPTH, PixelFormat::DataType::FLOAT, 32u);

// --- formats

constexpr auto FORMAT_R32_FLOAT = PixelFormat() << R32_FLOAT;
constexpr auto FORMAT_R32G32_FLOAT = PixelFormat() << R32_FLOAT << G32_FLOAT;
constexpr auto FORMAT_R32G32B32_FLOAT = PixelFormat() << R32_FLOAT << G32_FLOAT << B32_FLOAT;
constexpr auto FORMAT_R32G32B32A32_FLOAT = PixelFormat() << R32_FLOAT << G32_FLOAT << B32_FLOAT << A32_FLOAT;

constexpr auto FORMAT_R32_UINT = PixelFormat() << R32_UINT;

constexpr auto FORMAT_R8G8B8A8_UNORM_SRGB = PixelFormat() << R8_UNORM_SRGB << G8_UNORM_SRGB << B8_UNORM_SRGB << A8_UNORM_SRGB;

constexpr auto FORMAT_R8G8B8A8_UNORM = PixelFormat() << R8_UNORM << G8_UNORM << B8_UNORM << A8_UNORM;
constexpr auto FORMAT_B8G8R8A8_UNORM = PixelFormat() << B8_UNORM << G8_UNORM << R8_UNORM << A8_UNORM;
constexpr auto FORMAT_B8G8R8X8_UNORM = PixelFormat() << B8_UNORM << G8_UNORM << R8_UNORM << X8_UNORM;

constexpr auto FORMAT_D32_FLOAT = PixelFormat() << D32_FLOAT;

// --- lookup

constexpr const auto FORMAT_BY_ID = essentials::makeArray(
	std::make_pair(PixelFormatId::R32_FLOAT, FORMAT_R32_FLOAT)
	);

constexpr const PixelFormat& getFormatById(PixelFormatId pixelFormatId) {
	for (const auto& entry : detail::FORMAT_BY_ID) {
		if (entry.first == pixelFormatId) {
			return entry.second;
		}
	}

	throw dormouse_engine::exceptions::LogicError("Unexpected pixel format id");
}

} // namespace detail

constexpr PixelFormat::PixelFormat(PixelFormatId pixelFormatId) :
	PixelFormat(detail::getFormatById(pixelFormatId))
{
}

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_PIXELFORMAT_HPP_ */
