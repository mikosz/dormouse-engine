#include "RenderState.hpp"

#include <cassert>
#include <unordered_map>
#include <vector>

#include "dormouse-engine/essentials/Singleton.hpp"
#include "dormouse-engine/essentials/hash-combine.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer::control;

namespace /* anonymous */ {
	
class RenderStateFactory final :
	public essentials::Singleton<RenderStateFactory>
{
public:

	// TODO: this should be easily achievable using factory::Factory, but it isn't
	size_t create(
		graphics::Device& graphicsDevice,
		const graphics::RenderState::Configuration& configuration
		)
	{
		auto it = index_.find(configuration);

		if (it == index_.end()) {
			it = index_.emplace_hint(it, configuration, instances_.size());
			instances_.emplace_back(graphicsDevice, configuration);
		}

		return it->second;
	}

	graphics::RenderState get(size_t id) const {
		assert(id < instances_.size());
		return instances_[id];
	}

	void clear() {
		instances_.clear();
		index_.clear();
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
			return
				lhs.cullMode == rhs.cullMode &&
				lhs.fillMode == rhs.fillMode &&
				lhs.frontCounterClockwise == rhs.frontCounterClockwise &&
				lhs.blendingEnabled == rhs.blendingEnabled
				;
		}

	};

	using Index = std::unordered_map<
		graphics::RenderState::Configuration,
		size_t,
		ConfigurationHash,
		ConfigurationEqual
		>;

	using Instances = std::vector<graphics::RenderState>;

	Index index_;

	Instances instances_;

};
	
} // anonymous namespace

void RenderState::initialiseSystem(graphics::Device& graphicsDevice) {
	graphicsDevice.addDeviceDestroyedHandler([]() { RenderStateFactory::instance()->clear(); });
}

RenderState::RenderState(
	graphics::Device& graphicsDevice,
	graphics::RenderState::Configuration configuration
	) :
	renderStateId_(RenderStateFactory::instance()->create(graphicsDevice, configuration))
{
}

graphics::RenderState RenderState::get() const {
	if (renderStateId_ == INVALID_RENDER_STATE_ID) {
		return {};
	} else {
		return RenderStateFactory::instance()->get(renderStateId_);
	}
}
