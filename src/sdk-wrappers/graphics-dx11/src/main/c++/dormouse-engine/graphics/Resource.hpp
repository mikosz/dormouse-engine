#ifndef _DORMOUSEENGINE_GRAPHICS_RESOURCE_HPP_
#define _DORMOUSEENGINE_GRAPHICS_RESOURCE_HPP_

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "detail/detailfwd.hpp"
#include "PixelFormat.hpp"

namespace dormouse_engine::graphics {

class Device;
class Buffer;
class Texture;

const auto RESOURCE_SLOT_COUNT_PER_SHADER = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;

class Resource {
public:

	using Id = std::uintptr_t;

	Resource() = default;

	Id id() const {
		return reinterpret_cast<Id>(resource_.get());
	}

protected:

	Resource(system::windows::COMWrapper<ID3D11Resource> resource) :
		resource_(std::move(resource))
	{
	}

private:

	system::windows::COMWrapper<ID3D11Resource> resource_;

	friend struct detail::Internals;

};

class ResourceView {
public:

	ResourceView() = default;

	ResourceView(const Texture& texture);

	ResourceView(const Buffer& buffer, PixelFormat elementFormat);

	Resource::Id resourceId() const;

private:

	system::windows::COMWrapper<ID3D11ShaderResourceView> resourceView_;

	friend struct detail::Internals;

};

class UnorderedAccessView {
public:

	UnorderedAccessView() = default;

	UnorderedAccessView(
		const Buffer& buffer,
		PixelFormat elementFormat,
		size_t firstElementIdx,
		size_t elementCount
		);

private:

	system::windows::COMWrapper<ID3D11UnorderedAccessView> uav_;

	friend struct detail::Internals;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_RESOURCE_HPP_ */
