#ifndef _DORMOUSEENGINE_RENDERER_OBJECTS_RENDERSTATEFACTORY_HPP_
#define _DORMOUSEENGINE_RENDERER_OBJECTS_RENDERSTATEFACTORY_HPP_

#include <unordered_map>

#include "dormouse-engine/essentials/Singleton.hpp"
#include "dormouse-engine/essentials/hash-combine.hpp"
#include "dormouse-engine/graphics/RenderState.hpp"

namespace dormouse_engine::renderer::objects {

struct RenderStatePreset {
	static constexpr graphics::RenderState::Configuration OPAQUE = {
		graphics::RenderState::CullMode::BACK,
		graphics::RenderState::FillMode::SOLID,
		false,
		false
		};
};

class RenderStateFactory final :
	public essentials::Singleton<RenderStateFactory>
{
public:

	// TODO: this should be easily achievable using factory::Factory, but it isn't
	graphics::RenderState create(
		graphics::Device& graphicsDevice,
		const graphics::RenderState::Configuration& configuration
		)
	{
		auto it = instances_.find(configuration);

		if (it == instances_.end()) {
			it = instances_.emplace_hint(it, configuration, graphicsDevice, configuration);
		}

		return it->second;
	}

private:

	struct ConfigurationHash {

		size_t operator()(const graphics::RenderState::Configuration& configuration) const {
			auto seed = size_t(0);
			essentials::hashCombine(seed, std::hash_value(configuration.cullMode));
			essentials::hashCombine(seed, std::hash_value(configuration.fillMode));
			essentials::hashCombine(seed, std::hash_value(configuration.frontCounterClockwise));
			essentials::hashCombine(seed, std::hash_value(configuration.blendingEnabled));
			return seed;
		}

	};

	struct ConfigurationEqual {

		bool operator()(const graphics::RenderState::Configuration& lhs,
			const graphics::RenderState::Configuration& rhs) const
		{
			return lhs.cullMode == rhs.cullMode &&
				lhs.fillMode == rhs.fillMode &&
				lhs.frontCounterClockwise == rhs.frontCounterClockwise &&
				lhs.blendingEnabled == rhs.blendingEnabled
				;
		}

	};

	using Instances = std::unordered_map<
		graphics::RenderState::Configuration,
		graphics::RenderState,
		ConfigurationHash,
		ConfigurationEqual
		>;

	Instances instances_;

};

} // namespace dormouse_engine::renderer::objects

#endif /* _DORMOUSEENGINE_RENDERER_OBJECTS_RENDERSTATEFACTORY_HPP_ */
