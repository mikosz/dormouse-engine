#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_IMAGE_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_IMAGE_HPP_

#include <vector>
#include <string>

#include <boost/filesystem/path.hpp>

#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/exceptions/RuntimeError.hpp"
#include "PixelFormat.hpp"

namespace dormouse_engine::graphics {

class ImageLoadingError : public dormouse_engine::exceptions::RuntimeError {
public:

	ImageLoadingError(const std::string& path, const std::string& message);

	ImageLoadingError(const std::string& path, const std::exception& cause);

	const std::string& name() const noexcept override {
		using namespace std::string_literals;
		static const auto NAME = "ImageLoadingError"s;
		return NAME;
	}

	const std::string& path() const {
		return path_;
	}

private:

	std::string path_;

	static std::string buildMessage(const std::string& path, const std::string& message);

};

class Image {
public:

	using Dimensions = std::pair<size_t, size_t>; // TODO: find a better type

	static Image load(essentials::ConstBufferView data, const boost::filesystem::path& path);

	Image(
		std::vector<std::uint8_t> pixels,
		Dimensions size,
		size_t arraySize,
		size_t mipLevels,
		PixelFormat pixelFormat
		) :
		pixels_(std::move(pixels)),
		size_(size),
		arraySize_(arraySize),
		mipLevels_(mipLevels),
		pixelFormat_(pixelFormat)
	{
	}

	void save(const boost::filesystem::path& path) const;

	essentials::ConstBufferView pixels() const {
		return essentials::viewBuffer(pixels_);
	}

	Dimensions size() const {
		return size_;
	}

	size_t arraySize() const noexcept {
		return arraySize_;
	}

	size_t mipLevels() const noexcept {
		return mipLevels_;
	}

	PixelFormat pixelFormat() const {
		return pixelFormat_;
	}

private:

	essentials::ByteVector pixels_;

	Dimensions size_;

	size_t arraySize_;

	size_t mipLevels_;

	PixelFormat pixelFormat_;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_IMAGE_HPP_ */
