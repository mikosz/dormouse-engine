#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_PIXELFORMAT_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_PIXELFORMAT_HPP_

#include <array>
#include <cstdint>
#include <algorithm>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/exceptions/LogicError.hpp"
#include "dormouse-engine/essentials/types.hpp"
#include "dormouse-engine/enums.hpp"

namespace dormouse_engine::graphics {

DE_ENUM_VALUES(
	PixelFormatId,
	(UNKNOWN)(DXGI_FORMAT_UNKNOWN)

	(R32_FLOAT)(DXGI_FORMAT_R32_FLOAT)
	(R32G32_FLOAT)(DXGI_FORMAT_R32G32_FLOAT)
	(R32G32B32_FLOAT)(DXGI_FORMAT_R32G32B32_FLOAT)
	(R32G32B32A32_FLOAT)(DXGI_FORMAT_R32G32B32A32_FLOAT)

	(R32_UINT)(DXGI_FORMAT_R32_UINT)

	(R8_UINT)(DXGI_FORMAT_R8_UINT)

	(R8G8B8A8_UNORM)(DXGI_FORMAT_R8G8B8A8_UNORM)
	(B8G8R8A8_UNORM)(DXGI_FORMAT_B8G8R8A8_UNORM)
	(B8G8R8X8_UNORM)(DXGI_FORMAT_B8G8R8X8_UNORM)

	(R8G8B8A8_UNORM_SRGB)(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)

	(BC1_UNORM)(DXGI_FORMAT_BC1_UNORM)

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

		(X)
		(Y)
		(Z)
		(W)

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
		std::uint8_t bitsPerPixel = 0u;

		constexpr Channel() = default;

		constexpr Channel(ChannelType type, DataType dataType, std::uint8_t bitsPerPixel) :
			type(type),
			dataType(dataType),
			bitsPerPixel(bitsPerPixel)
		{
		}

		constexpr bool operator==(const Channel& other) const {
			return 
				type == other.type &&
				dataType == other.dataType &&
				bitsPerPixel == other.bitsPerPixel
				;
		}

		constexpr bool operator!=(const Channel& other) const {
			return !(*this == other);
		}

	};

	constexpr PixelFormat() = default;

	constexpr PixelFormat(PixelFormatId pixelFormatId);

	constexpr PixelFormat(Channel first) :
		channels_{ std::move(first), Channel(), Channel(), Channel() }
	{
	}

	constexpr PixelFormat(const PixelFormat& other, Channel next) :
		channels_(other.channels_)
	{
		const auto index = channelsUsed();

		if (index >= MAX_CHANNELS) {
			throw exceptions::LogicError("Couldn't find an unused channel");
		}

		if (channels_[index].type == ChannelType::UNUSED) {
			channels_[index] = std::move(next);
			return;
		}
	}

	constexpr bool operator==(const PixelFormat& other) const {
		for (size_t i = 0; i < PixelFormat::MAX_CHANNELS; ++i) {
			if (channels_[i] != other.channels_[i]) {
				return false;
			}
		}

		return true;
	}

	constexpr bool operator!=(const PixelFormat& other) const {
		return !(*this == other);
	}

	constexpr PixelFormatId id() const;

	constexpr size_t pixelSize() const {
		auto bits = size_t(0);

		for (size_t i = 0; i < MAX_CHANNELS; ++i) {
			bits += channels_[i].bitsPerPixel;
		}

		return (bits + 7) / 8;
	}

	constexpr size_t rowPitch(size_t width) const;

	constexpr size_t slicePitch(size_t height, size_t rowPitch) const;

	constexpr const Channel& channel(size_t index) const {
		if (index >= channelsUsed()) {
			throw exceptions::LogicError("Invalid channel index");
		}

		return channels_[index];
	}

	constexpr size_t channelsUsed() const {
		for (size_t i = 0; i < MAX_CHANNELS; ++i) {
			if (channels_[i].type == ChannelType::UNUSED) {
				return i;
			}
		}
		return MAX_CHANNELS;
	}

private:

	static const size_t MAX_CHANNELS = 4u;

	std::array<Channel, MAX_CHANNELS> channels_;

};

constexpr PixelFormat operator<<(PixelFormat lhs, PixelFormat::Channel rhs) {
	return PixelFormat(lhs, rhs);
}

// --- channels

constexpr auto X_UNKNOWN = PixelFormat::Channel(PixelFormat::ChannelType::RESERVED, PixelFormat::DataType::UNKNOWN, 0u);

constexpr auto R32_FLOAT = PixelFormat::Channel(PixelFormat::ChannelType::RED, PixelFormat::DataType::FLOAT, 32u);
constexpr auto G32_FLOAT = PixelFormat::Channel(PixelFormat::ChannelType::GREEN, PixelFormat::DataType::FLOAT, 32u);
constexpr auto B32_FLOAT = PixelFormat::Channel(PixelFormat::ChannelType::BLUE, PixelFormat::DataType::FLOAT, 32u);
constexpr auto A32_FLOAT = PixelFormat::Channel(PixelFormat::ChannelType::ALPHA, PixelFormat::DataType::FLOAT, 32u);

constexpr auto X32_FLOAT = PixelFormat::Channel(PixelFormat::ChannelType::X, PixelFormat::DataType::FLOAT, 32u);
constexpr auto Y32_FLOAT = PixelFormat::Channel(PixelFormat::ChannelType::Y, PixelFormat::DataType::FLOAT, 32u);
constexpr auto Z32_FLOAT = PixelFormat::Channel(PixelFormat::ChannelType::Z, PixelFormat::DataType::FLOAT, 32u);
constexpr auto W32_FLOAT = PixelFormat::Channel(PixelFormat::ChannelType::W, PixelFormat::DataType::FLOAT, 32u);

constexpr auto R8_UINT = PixelFormat::Channel(PixelFormat::ChannelType::RED, PixelFormat::DataType::UINT, 8u);

constexpr auto R32_UINT = PixelFormat::Channel(PixelFormat::ChannelType::RED, PixelFormat::DataType::UINT, 32u);

constexpr auto R16_UINT = PixelFormat::Channel(PixelFormat::ChannelType::RED, PixelFormat::DataType::UINT, 16u);

constexpr auto R8_UNORM_SRGB = PixelFormat::Channel(PixelFormat::ChannelType::RED, PixelFormat::DataType::UNORM_SRGB, 8u);
constexpr auto G8_UNORM_SRGB = PixelFormat::Channel(PixelFormat::ChannelType::GREEN, PixelFormat::DataType::UNORM_SRGB, 8u);
constexpr auto B8_UNORM_SRGB = PixelFormat::Channel(PixelFormat::ChannelType::BLUE, PixelFormat::DataType::UNORM_SRGB, 8u);
constexpr auto A8_UNORM_SRGB = PixelFormat::Channel(PixelFormat::ChannelType::ALPHA, PixelFormat::DataType::UNORM_SRGB, 8u);

constexpr auto R8_UNORM = PixelFormat::Channel(PixelFormat::ChannelType::RED, PixelFormat::DataType::UNORM, 8u);
constexpr auto G8_UNORM = PixelFormat::Channel(PixelFormat::ChannelType::GREEN, PixelFormat::DataType::UNORM, 8u);
constexpr auto B8_UNORM = PixelFormat::Channel(PixelFormat::ChannelType::BLUE, PixelFormat::DataType::UNORM, 8u);
constexpr auto A8_UNORM = PixelFormat::Channel(PixelFormat::ChannelType::ALPHA, PixelFormat::DataType::UNORM, 8u);
constexpr auto X8_UNORM = PixelFormat::Channel(PixelFormat::ChannelType::RESERVED, PixelFormat::DataType::UNORM, 8u);

constexpr auto BC1_UNORM = PixelFormat::Channel(PixelFormat::ChannelType::COMPRESSION_BLOCK, PixelFormat::DataType::UNORM, 8u);

constexpr auto D32_FLOAT = PixelFormat::Channel(PixelFormat::ChannelType::DEPTH, PixelFormat::DataType::FLOAT, 32u);

// --- formats

constexpr auto FORMAT_UNKNOWN = PixelFormat() << X_UNKNOWN;

constexpr auto FORMAT_R32_FLOAT = PixelFormat() << R32_FLOAT;
constexpr auto FORMAT_R32G32_FLOAT = PixelFormat() << R32_FLOAT << G32_FLOAT;
constexpr auto FORMAT_R32G32B32_FLOAT = PixelFormat() << R32_FLOAT << G32_FLOAT << B32_FLOAT;
constexpr auto FORMAT_R32G32B32A32_FLOAT = PixelFormat() << R32_FLOAT << G32_FLOAT << B32_FLOAT << A32_FLOAT;

constexpr auto FORMAT_X32_FLOAT = PixelFormat() << X32_FLOAT;
constexpr auto FORMAT_X32Y32_FLOAT = PixelFormat() << X32_FLOAT << Y32_FLOAT;
constexpr auto FORMAT_X32Y32Z32_FLOAT = PixelFormat() << X32_FLOAT << Y32_FLOAT << Z32_FLOAT;
constexpr auto FORMAT_X32Y32Z32W32_FLOAT = PixelFormat() << X32_FLOAT << Y32_FLOAT << Z32_FLOAT << W32_FLOAT;

constexpr auto FORMAT_R8_UINT = PixelFormat() << R8_UINT;

constexpr auto FORMAT_R32_UINT = PixelFormat() << R32_UINT;

constexpr auto FORMAT_R16_UINT = PixelFormat() << R16_UINT;

constexpr auto FORMAT_R8G8B8A8_UNORM_SRGB = PixelFormat() << R8_UNORM_SRGB << G8_UNORM_SRGB << B8_UNORM_SRGB << A8_UNORM_SRGB;

constexpr auto FORMAT_R8G8B8A8_UNORM = PixelFormat() << R8_UNORM << G8_UNORM << B8_UNORM << A8_UNORM;
constexpr auto FORMAT_B8G8R8A8_UNORM = PixelFormat() << B8_UNORM << G8_UNORM << R8_UNORM << A8_UNORM;
constexpr auto FORMAT_B8G8R8X8_UNORM = PixelFormat() << B8_UNORM << G8_UNORM << R8_UNORM << X8_UNORM;

constexpr auto FORMAT_BC1_UNORM = PixelFormat() << BC1_UNORM;

constexpr auto FORMAT_D32_FLOAT = PixelFormat() << D32_FLOAT;

// --- lookup

namespace detail {

constexpr const auto FORMAT_BY_ID = essentials::makeArray(
	std::make_pair(PixelFormatId::UNKNOWN, FORMAT_UNKNOWN),

	std::make_pair(PixelFormatId::R32_FLOAT, FORMAT_R32_FLOAT),
	std::make_pair(PixelFormatId::R32G32_FLOAT, FORMAT_R32G32_FLOAT),
	std::make_pair(PixelFormatId::R32G32B32_FLOAT, FORMAT_R32G32B32_FLOAT),
	std::make_pair(PixelFormatId::R32G32B32A32_FLOAT, FORMAT_R32G32B32A32_FLOAT),

	std::make_pair(PixelFormatId::R32_FLOAT, FORMAT_X32_FLOAT),
	std::make_pair(PixelFormatId::R32G32_FLOAT, FORMAT_X32Y32_FLOAT),
	std::make_pair(PixelFormatId::R32G32B32_FLOAT, FORMAT_X32Y32Z32_FLOAT),
	std::make_pair(PixelFormatId::R32G32B32A32_FLOAT, FORMAT_X32Y32Z32W32_FLOAT),

	std::make_pair(PixelFormatId::R8_UINT, FORMAT_R8_UINT),

	std::make_pair(PixelFormatId::R32_UINT, FORMAT_R32_UINT),

	std::make_pair(PixelFormatId::R8G8B8A8_UNORM, FORMAT_R8G8B8A8_UNORM),
	std::make_pair(PixelFormatId::B8G8R8A8_UNORM, FORMAT_B8G8R8A8_UNORM),
	std::make_pair(PixelFormatId::B8G8R8X8_UNORM, FORMAT_B8G8R8X8_UNORM),

	std::make_pair(PixelFormatId::R8G8B8A8_UNORM_SRGB, FORMAT_R8G8B8A8_UNORM_SRGB),

	std::make_pair(PixelFormatId::BC1_UNORM, FORMAT_BC1_UNORM),

	std::make_pair(PixelFormatId::D32_FLOAT, FORMAT_D32_FLOAT)
	);

constexpr inline const PixelFormat& getFormatById(PixelFormatId pixelFormatId) {
	for (const auto& entry : detail::FORMAT_BY_ID) {
		if (entry.first == pixelFormatId) {
			return entry.second;
		}
	}

	throw dormouse_engine::exceptions::LogicError("Unexpected pixel format id");
}

constexpr inline PixelFormatId getIdByFormat(const PixelFormat& pixelFormat) {
	for (const auto& entry : detail::FORMAT_BY_ID) {
		if (entry.second == pixelFormat) {
			return entry.first;
		}
	}

	throw dormouse_engine::exceptions::LogicError("Unexpected pixel format");
}

constexpr inline size_t blockCount(PixelFormat::ChannelType channelType, size_t width) {
	switch (channelType) {
	case PixelFormat::ChannelType::COMPRESSION_BLOCK:
		return (width + 3) / 4;
	default:
		return width;
	}
}

} // namespace detail

constexpr inline PixelFormat::PixelFormat(PixelFormatId pixelFormatId) :
	PixelFormat(detail::getFormatById(pixelFormatId))
{
}

constexpr inline PixelFormatId PixelFormat::id() const {
	return detail::getIdByFormat(*this);
}

constexpr inline size_t PixelFormat::rowPitch(size_t width) const {
	return detail::blockCount(channels_[0].type, width) * pixelSize();
}

constexpr inline size_t PixelFormat::slicePitch(size_t height, size_t rowPitch) const {
	return detail::blockCount(channels_[0].type, height) * rowPitch;
}

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_PIXELFORMAT_HPP_ */
