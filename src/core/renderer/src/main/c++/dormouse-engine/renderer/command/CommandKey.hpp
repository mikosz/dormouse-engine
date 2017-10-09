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
constexpr auto FULLSCREEN_LAYER_BITS = 2u;

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
constexpr auto VIEWPORT_LAYER_BITS = 2u;

enum class TranslucencyType {
	OPAQUE,
	TRANSLUCENT,
};
constexpr auto TRANSLUCENCY_TYPE_BITS = 1u;

using Depth = std::int32_t;
constexpr auto DEPTH_BITS = 24u;

using MaterialId = std::uint32_t;
constexpr auto MATERIAL_ID_BITS = 30u;

union CommandKey {
	using Hash = std::uint64_t;

	struct {
		FullscreenLayerId fullscreenLayerId : FULLSCREEN_LAYER_BITS;
		ViewportId viewportId : VIEWPORT_ID_BITS;
		ViewportLayer viewportLayer : VIEWPORT_LAYER_BITS;
		TranslucencyType translucencyType : TRANSLUCENCY_TYPE_BITS;
		Depth depth : DEPTH_BITS;
		MaterialId materialId : MATERIAL_ID_BITS;
	} attributes;

	Hash hash;
};

static_assert(
	FULLSCREEN_LAYER_BITS +
	VIEWPORT_ID_BITS +
	VIEWPORT_LAYER_BITS +
	TRANSLUCENCY_TYPE_BITS +
	DEPTH_BITS +
	MATERIAL_ID_BITS
	<
	sizeof(CommandKey) * 8u
	);

static_assert(sizeof(CommandKey) == sizeof(CommandKey::Hash));

} // namespace dormouse_engine::renderer::command

#endif /* _DORMOUSEENGINE_RENDERER_COMMAND_COMMANDKEY_HPP_ */
