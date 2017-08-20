#include "Image.hpp"

#include <algorithm>
#include <iterator>

#include <boost/algorithm/string/predicate.hpp>

#include <DirectXTex/DirectXTex.h>

#include "dormouse-engine/logger.hpp"
#include "dormouse-engine/system/windows/Error.hpp"
#include "PixelFormat.hpp"

DE_LOGGER_CATEGORY("COCONUT.MILK.GRAPHICS");

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
	auto result = "Failed to load image \"" + path + "\"";
	if (!message.empty()) {
		result += ": " + message;
	}

	return result;
}

Image Image::load(essentials::BufferView data, const std::string& path) {
	try {
		DE_LOG_DEBUG << "Loading image " << path;

		auto metadata = DirectX::TexMetadata();
		auto scratchImage = DirectX::ScratchImage();

		if (boost::algorithm::ends_with(path, ".tga")) { // TODO: refactor?
			system::windows::checkSystemCall(
				DirectX::LoadFromTGAMemory(data.bytes(), data.size(), &metadata, scratchImage),
				"Failed to load a targa file"
			);
		} else if (boost::algorithm::ends_with(path, ".dds")) {
			system::windows::checkSystemCall(
				DirectX::LoadFromDDSMemory(
					data.bytes(),
					data.size(),
					DirectX::DDS_FLAGS_NONE,
					&metadata,
					scratchImage
					),
				"Failed to load a dds file"
				);
		} else if (boost::algorithm::ends_with(path, ".bmp")) {
			system::windows::checkSystemCall(
				DirectX::LoadFromWICMemory( // TODO: definitely refactor
					data.bytes(),
					data.size(),
					DirectX::WIC_CODEC_BMP,
					&metadata,
					scratchImage
					),
				"Failed to load a bmp file"
				);
		} else if (boost::algorithm::ends_with(path, ".png")) {
			system::windows::checkSystemCall(
				DirectX::LoadFromWICMemory( // TODO: definitely refactor
					data.bytes(),
					data.size(),
					DirectX::WIC_CODEC_PNG,
					&metadata,
					scratchImage
					),
				"Failed to load a png file"
				);
		} else {
			throw std::runtime_error("Unsupported image format extension: " + path);
		}

		auto pixels = essentials::ByteVector();
		pixels.reserve(scratchImage.GetPixelsSize());
		std::copy(
			scratchImage.GetPixels(),
			scratchImage.GetPixels() + scratchImage.GetPixelsSize(),
			std::back_inserter(pixels)
			);

		if (allPixelFormatValues().count(static_cast<PixelFormat>(metadata.format)) == 0) {
			throw std::runtime_error("Unsupported image format");
		}

		return Image(
			std::move(pixels),
			{ metadata.width, metadata.height },
			metadata.arraySize,
			metadata.mipLevels,
			static_cast<PixelFormat>(metadata.format)
			);
	} catch (const std::exception& e) {
		throw ImageLoadingError(path, e);
	}
}
