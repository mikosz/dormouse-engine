#include "RenderTargetView.hpp"

#include <cassert>
#include <unordered_map>
#include <vector>

#include "dormouse-engine/essentials/Singleton.hpp"
#include "dormouse-engine/essentials/hash-combine.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer::control;

namespace /* anonymous */ {

class RenderTargetViewFactory final :
	public essentials::Singleton<RenderTargetViewFactory>
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

	graphics::RenderTargetView get(size_t id) const {
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

	using Instances = std::vector<graphics::RenderTargetView>;

	Index index_;

	Instances instances_;

};

} // anonymous namespace

void RenderTargetView::initialiseSystem(graphics::Device& graphicsDevice) {
	graphicsDevice.addDeviceDestroyedHandler([]() { RenderTargetViewFactory::instance()->clear(); });
}

RenderTargetView::RenderTargetView(const graphics::Texture& texture) :
	renderTargetViewId_(RenderTargetViewFactory::instance()->create(texture))
{
}

graphics::RenderTargetView RenderTargetView::get() const {
	if (renderTargetViewId_ == INVALID_RENDERTARGET_VIEW_ID) {
		return graphics::RenderTargetView();
	} else {
		return RenderTargetViewFactory::instance()->get(renderTargetViewId_);
	}
}
