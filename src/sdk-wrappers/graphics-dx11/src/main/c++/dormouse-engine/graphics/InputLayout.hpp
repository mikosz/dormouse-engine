#ifndef _DORMOUSEENGINE_GRAPHICS_INPUTLAYOUTDESCRIPTION_HPP_
#define _DORMOUSEENGINE_GRAPHICS_INPUTLAYOUTDESCRIPTION_HPP_

#include <vector>
#include <functional>

#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/enums.hpp"
#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "detail/detailfwd.hpp"
#include "PixelFormat.hpp"

namespace dormouse_engine::graphics {

class Device;

class InputLayout final {
public:

	DE_MEMBER_ENUM_VALUES(
		SlotType,
		(PER_VERTEX_DATA)(D3D11_INPUT_PER_VERTEX_DATA)
		(PER_INSTANCE_DATA)(D3D11_INPUT_PER_INSTANCE_DATA)
		);

	struct Element {
	public:

		Element(
			std::string semantic,
			size_t semanticIndex,
			PixelFormat format,
			SlotType inputSlotType,
			size_t instanceDataStepRate
			);

		std::string semantic;

		size_t semanticIndex;

		PixelFormat format;

		SlotType inputSlotType;

		size_t instanceDataStepRate;

	};

	using Elements = std::vector<Element>;

	InputLayout() = default;

	InputLayout(
		Device& renderer,
		const Elements& elements
		);

private:

	system::windows::COMWrapper<ID3D11InputLayout> inputLayout_;

	friend struct detail::Internals;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_INPUTLAYOUTDESCRIPTION_HPP_ */
