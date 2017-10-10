#include "graphics.pch.hpp"

#include "Image.hpp"

#include <algorithm>
#include <iterator>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <DirectXTex/DirectXTex.h>

#include "dormouse-engine/logger.hpp"
#include "dormouse-engine/system/windows/Error.hpp"
#include "PixelFormat.hpp"

DE_LOGGER_CATEGORY("DORMOUSE_ENGINE.GRAPHICS");

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

using namespace std::string_literals;

ImageLoadingError::ImageLoadingError(const std::string& path, const std::string& message) :
	dormouse_engine::exceptions::RuntimeError(buildMessage(path, message)),
	path_(path)
{
}

ImageLoadingError::ImageLoadingError(const std::string& path, const std::exception& cause) :
	dormouse_engine::exceptions::RuntimeError(buildMessage(path, std::string()), cause),
	path_(path)
{
}

std::string ImageLoadingError::buildMessage(const std::string& path, const std::string& message) {
	auto result = R"(Failed to load image ")" + path + R"(")";
	if (!message.empty()) {
		result += ": " + message;
	}

	return result;
}

Image Image::load(essentials::ConstBufferView data, const boost::filesystem::path& path) {
	try {
		DE_LOG_DEBUG << "Loading image " << path;

		auto metadata = DirectX::TexMetadata();
		auto scratchImage = DirectX::ScratchImage();

		if (boost::algorithm::ends_with(path.string(), ".tga")) { // TODO: refactor?
			system::windows::checkSystemCall(
				DirectX::LoadFromTGAMemory(data.data(), data.size(), &metadata, scratchImage),
				"Failed to load a targa file"
			);
		} else if (boost::algorithm::ends_with(path.string(), ".dds")) {
			system::windows::checkSystemCall(
				DirectX::LoadFromDDSMemory(
					data.data(),
					data.size(),
					DirectX::DDS_FLAGS_NONE,
					&metadata,
					scratchImage
					),
				"Failed to load a dds file"
				);
		} else if (boost::algorithm::ends_with(path.string(), ".bmp")) {
			system::windows::checkSystemCall(
				DirectX::LoadFromWICMemory( // TODO: definitely refactor
					data.data(),
					data.size(),
					DirectX::WIC_CODEC_BMP,
					&metadata,
					scratchImage
					),
				"Failed to load a bmp file"
				);
		} else if (boost::algorithm::ends_with(path.string(), ".png")) {
			system::windows::checkSystemCall(
				DirectX::LoadFromWICMemory( // TODO: definitely refactor
					data.data(),
					data.size(),
					DirectX::WIC_CODEC_PNG,
					&metadata,
					scratchImage
					),
				"Failed to load a png file"
				);
		} else {
			throw std::runtime_error("Unsupported image format extension: " + path.string());
		}

		auto pixels = essentials::ByteVector();
		pixels.reserve(scratchImage.GetPixelsSize());
		std::copy(
			scratchImage.GetPixels(),
			scratchImage.GetPixels() + scratchImage.GetPixelsSize(),
			std::back_inserter(pixels)
			);

		auto pixelFormatId = PixelFormatId();
		fromIntegral(pixelFormatId, static_cast<std::underlying_type_t<decltype(metadata.format)>>(metadata.format));

		return Image(
			std::move(pixels),
			{ metadata.width, metadata.height },
			metadata.arraySize,
			metadata.mipLevels,
			PixelFormat(pixelFormatId)
			);
	} catch (const std::exception& e) {
		throw ImageLoadingError(path.string(), e);
	}
}

void Image::save(const boost::filesystem::path& path, size_t rowPitch) const {
	if (path.has_parent_path()) {
		boost::filesystem::create_directories(path.parent_path());
	}

	auto dxImage = DirectX::Image();
	dxImage.format = static_cast<DXGI_FORMAT>(pixelFormat_.id());
	dxImage.width = size_.first;
	dxImage.height = size_.second;
	dxImage.pixels = const_cast<essentials::Byte*>(pixels_.data());
	dxImage.rowPitch = rowPitch;
	dxImage.slicePitch = pixelFormat_.slicePitch(dxImage.height, rowPitch);

	system::windows::checkSystemCall(
		DirectX::SaveToTGAFile(dxImage, path.wstring().c_str()),
		"Failed to save a tga file to " + path.string()
		);
}
