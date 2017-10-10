#include "Viewport.hpp"

#include <cassert>
#include <unordered_map>
#include <vector>

#include "dormouse-engine/essentials/Singleton.hpp"
#include "dormouse-engine/essentials/hash-combine.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer::control;

namespace /* anonymous */ {

class ViewportFactory final :
	public essentials::Singleton<ViewportFactory>
{
public:

	// TODO: this should be easily achievable using factory::Factory, but it isn't
	// TODO: this behaviour is now duplicated throught renderer::control
	size_t create(const graphics::Viewport::Configuration& configuration) {
		auto it = index_.find(configuration);

		if (it == index_.end()) {
			it = index_.emplace_hint(it, configuration, instances_.size());
			instances_.emplace_back(configuration);
		}

		return it->second;
	}

	graphics::Viewport get(size_t id) const {
		assert(id < instances_.size());
		return instances_[id];
	}

	void clear() {
		instances_.clear();
		index_.clear();
	}

private:

	struct ConfigurationHash {

		size_t operator()(const graphics::Viewport::Configuration& configuration) const {
			auto seed = size_t(0);
			essentials::hashCombine(seed, std::hash_value(configuration.height));
			essentials::hashCombine(seed, std::hash_value(configuration.width));
			essentials::hashCombine(seed, std::hash_value(configuration.minDepth));
			essentials::hashCombine(seed, std::hash_value(configuration.maxDepth));
			essentials::hashCombine(seed, std::hash_value(configuration.topLeftX));
			essentials::hashCombine(seed, std::hash_value(configuration.topLeftY));
			return seed;
		}

	};

	struct ConfigurationEqual {

		bool operator()(const graphics::Viewport::Configuration& lhs,
			const graphics::Viewport::Configuration& rhs) const
		{
			return
				lhs.height == rhs.height &&
				lhs.width == rhs.width &&
				lhs.minDepth == rhs.minDepth &&
				lhs.maxDepth == rhs.maxDepth &&
				lhs.topLeftX == rhs.topLeftX &&
				lhs.topLeftY == rhs.topLeftY
				;
		}

	};

	using Index = std::unordered_map<
		graphics::Viewport::Configuration,
		size_t,
		ConfigurationHash,
		ConfigurationEqual
	>;

	using Instances = std::vector<graphics::Viewport>;

	Index index_;

	Instances instances_;

};

} // anonymous namespace

Viewport::Viewport(const graphics::Viewport::Configuration& configuration) :
	viewportObjectId_(ViewportFactory::instance()->create(configuration))
{
}

graphics::Viewport Viewport::get() const {
	assert(viewportObjectId_ != INVALID_VIEWPORT_OBJECT_ID);
	return ViewportFactory::instance()->get(viewportObjectId_);
}
