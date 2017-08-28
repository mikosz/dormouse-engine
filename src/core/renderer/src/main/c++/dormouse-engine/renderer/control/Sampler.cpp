#include "Sampler.hpp"

#include <cassert>
#include <unordered_map>
#include <vector>

#include "dormouse-engine/essentials/Singleton.hpp"
#include "dormouse-engine/essentials/hash-combine.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer::control;

namespace /* anonymous */ {

class SamplerFactory final :
	public essentials::Singleton<SamplerFactory>
{
public:

	// TODO: this should be easily achievable using factory::Factory, but it isn't
	// TODO: this behaviour is now duplicated throught renderer::control
	size_t create(
		graphics::Device& graphicsDevice,
		const graphics::Sampler::Configuration& configuration
		)
	{
		auto it = index_.find(configuration);

		if (it == index_.end()) {
			it = index_.emplace_hint(it, configuration, instances_.size());
			instances_.emplace_back(graphicsDevice, configuration);
		}

		return it->second;
	}

	graphics::Sampler get(size_t id) const {
		assert(id < instances_.size());
		return instances_[id];
	}

private:

	struct ConfigurationHash {

		size_t operator()(const graphics::Sampler::Configuration& configuration) const {
			auto seed = size_t(0);
			essentials::hashCombine(seed, std::hash_value(configuration.addressModeU));
			essentials::hashCombine(seed, std::hash_value(configuration.addressModeV));
			essentials::hashCombine(seed, std::hash_value(configuration.addressModeW));
			essentials::hashCombine(seed, std::hash_value(configuration.filter));
			return seed;
		}

	};

	struct ConfigurationEqual {

		bool operator()(const graphics::Sampler::Configuration& lhs,
			const graphics::Sampler::Configuration& rhs) const
		{
			return
				lhs.addressModeU == rhs.addressModeU &&
				lhs.addressModeV == rhs.addressModeV &&
				lhs.addressModeW == rhs.addressModeW &&
				lhs.filter == rhs.filter
				;
		}

	};

	using Index = std::unordered_map<
		graphics::Sampler::Configuration,
		size_t,
		ConfigurationHash,
		ConfigurationEqual
		>;

	using Instances = std::vector<graphics::Sampler>;

	Index index_;

	Instances instances_;

};

} // anonymous namespace

Sampler::Sampler(
	graphics::Device& graphicsDevice,
	graphics::Sampler::Configuration configuration
	) :
	samplerId_(SamplerFactory::instance()->create(graphicsDevice, configuration))
{
}

void Sampler::bind(graphics::CommandList& commandList, graphics::ShaderType stage, size_t slot) const {
	commandList.setSampler(SamplerFactory::instance()->get(samplerId_), stage, slot);
}
