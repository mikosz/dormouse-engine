#include "DepthStencilView.hpp"

#include <cassert>
#include <unordered_map>
#include <vector>

#include "dormouse-engine/essentials/Singleton.hpp"
#include "dormouse-engine/essentials/hash-combine.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer::control;

namespace /* anonymous */ {

class DepthStencilViewFactory final :
	public essentials::Singleton<DepthStencilViewFactory>
{
public:

	size_t create(const graphics::Texture& texture)
	{
		const auto key = std::make_tuple(texture.id(), texture.pixelFormat());
		auto it = index_.find(key);

		if (it == index_.end()) {
			it = index_.emplace_hint(it, key, instances_.size());
			instances_.emplace_back(texture);
		}

		return it->second;
	}

	graphics::DepthStencilView get(size_t id) const {
		assert(id < instances_.size());
		return instances_[id];
	}

	void clear() {
		instances_.clear();
		index_.clear();
	}

private:

	using IndexKey = std::tuple<graphics::Resource::Id, graphics::PixelFormat>;

	struct KeyHash {

		size_t operator()(const IndexKey& key) const {
			auto seed = size_t(0);
			essentials::hashCombine(seed, std::hash_value(std::get<graphics::Resource::Id>(key)));
			essentials::hashCombine(seed, std::hash_value(std::get<graphics::PixelFormat>(key).id()));
			return seed;
		}

	};

	using Index = std::unordered_map<
		IndexKey,
		size_t,
		KeyHash
		>;

	using Instances = std::vector<graphics::DepthStencilView>;

	Index index_;

	Instances instances_;

};

} // anonymous namespace

void DepthStencilView::initialiseSystem(graphics::Device& graphicsDevice) {
	graphicsDevice.addDeviceDestroyedHandler([]() { DepthStencilViewFactory::instance()->clear(); });
}

DepthStencilView::DepthStencilView(const graphics::Texture& texture) :
	depthStencilViewId_(DepthStencilViewFactory::instance()->create(texture))
{
}

graphics::DepthStencilView DepthStencilView::get() const {
	if (depthStencilViewId_ == INVALID_DEPTHSTENCIL_VIEW_ID) {
		return graphics::DepthStencilView();
	} else {
		return DepthStencilViewFactory::instance()->get(depthStencilViewId_);
	}
}
