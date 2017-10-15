#ifndef _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDKEY_HPP_
#define _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDKEY_HPP_

#include <cstdint>
#include <type_traits>

namespace dormouse_engine::renderer::command {

enum class FullscreenLayerId {
	GAME,
	FULLSCREEN_EFFECT,
	HUD,
	DEBUG,
};
constexpr auto FULLSCREEN_LAYER_BITS = 3u;

enum class ViewportId {
	FULLSCREEN,
};
constexpr auto VIEWPORT_ID_BITS = 1u;

enum class ViewportLayer {
	SKYBOX,
	WORLD,
	EFFECT,
	HUD,
};
constexpr auto VIEWPORT_LAYER_BITS = 3u;

enum class TranslucencyType {
	OPAQUE,
	TRANSLUCENT,
};
constexpr auto TRANSLUCENCY_TYPE_BITS = 1u;

using Depth = std::int32_t;
constexpr auto DEPTH_BITS = 24u;

using MaterialId = std::uint32_t;
constexpr auto MATERIAL_ID_BITS = 32u;

class CommandKey final {
public:

	constexpr CommandKey() = default;

	constexpr CommandKey(
		FullscreenLayerId fullscreenLayerId,
		ViewportId viewportId,
		ViewportLayer viewportLayer,
		TranslucencyType translucencyType,
		Depth depth,
		MaterialId materialId
		) noexcept :
		data_(fullscreenLayerId, viewportId, viewportLayer, translucencyType, depth, materialId)
	{
	}

	[[nodiscard]] const std::uint64_t hash() const noexcept {
		return reinterpret_cast<const std::uint64_t&>(data_);
	}

	constexpr void setFullscreenLayerId(FullscreenLayerId fullscreenLayerId) noexcept {
		data_.fullscreenLayerId = fullscreenLayerId;
	}

	constexpr void setViewportId(ViewportId viewportId) noexcept {
		data_.viewportId = viewportId;
	}

	constexpr void setViewportLayer(ViewportLayer viewportLayer) noexcept {
		data_.viewportLayer = viewportLayer;
	}

	constexpr void setTranslucencyType(TranslucencyType translucencyType) noexcept {
		data_.translucencyType = translucencyType;
	}

	constexpr void setDepth(Depth depth) noexcept {
		data_.depth = depth;
	}

	constexpr void setMaterialId(MaterialId materialId) noexcept {
		data_.materialId = materialId;
	}

private:

	struct Data {

		// TODO: ordering here is reversed because visual studio orders bitfield elements from right to left
		// but this won't work on other platforms. Find a better way.
		MaterialId materialId : MATERIAL_ID_BITS;
		Depth depth : DEPTH_BITS;
		TranslucencyType translucencyType : TRANSLUCENCY_TYPE_BITS;
		ViewportLayer viewportLayer : VIEWPORT_LAYER_BITS;
		ViewportId viewportId : VIEWPORT_ID_BITS;
		FullscreenLayerId fullscreenLayerId : FULLSCREEN_LAYER_BITS;

		constexpr Data() noexcept :
			Data(
				static_cast<FullscreenLayerId>(0),
				static_cast<ViewportId>(0),
				static_cast<ViewportLayer>(0),
				static_cast<TranslucencyType>(0),
				0,
				0
				)
		{
		}

		constexpr Data(
			FullscreenLayerId fullscreenLayerId,
			ViewportId viewportId,
			ViewportLayer viewportLayer,
			TranslucencyType translucencyType,
			Depth depth,
			MaterialId materialId
			) noexcept :
			materialId(materialId),
			depth(depth),
			translucencyType(translucencyType),
			viewportLayer(viewportLayer),
			viewportId(viewportId),
			fullscreenLayerId(fullscreenLayerId)
		{
		}

	} data_;

	friend bool operator<(const CommandKey& lhs, const CommandKey& rhs) noexcept {
		return rhs.hash() < lhs.hash();
	}

};

static_assert(sizeof(CommandKey) == sizeof(std::uint64_t));

} // namespace dormouse_engine::renderer::command

namespace std {

template <>
struct hash<dormouse_engine::renderer::command::CommandKey> {

	constexpr size_t operator()(const dormouse_engine::renderer::command::CommandKey commandKey) const noexcept {
		return commandKey.hash();
	}

};

} // namespace std

#endif /* _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDKEY_HPP_ */
