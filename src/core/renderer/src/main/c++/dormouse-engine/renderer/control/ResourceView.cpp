#include "ResourceView.hpp"

#include <cassert>
#include <unordered_map>
#include <vector>

#include "dormouse-engine/essentials/Singleton.hpp"
#include "dormouse-engine/essentials/hash-combine.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer::control;

namespace /* anonymous */ {

class ResourceViewFactory final :
	public essentials::Singleton<ResourceViewFactory>
{
public:

	// TODO: this should be easily achievable using factory::Factory, but it isn't
	// TODO: this behaviour is now duplicated throught renderer::control
	size_t create(const graphics::Buffer& buffer, graphics::PixelFormat elementFormat)
	{
		const auto key = std::make_tuple(buffer.id(), elementFormat);
		auto it = index_.find(key);

		if (it == index_.end()) {
			it = index_.emplace_hint(it, key, instances_.size());
			instances_.emplace_back(buffer, elementFormat);
		}

		return it->second;
	}

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

	graphics::ResourceView get(size_t id) const {
		assert(id < instances_.size());
		return instances_[id];
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

	using Instances = std::vector<graphics::ResourceView>;

	Index index_;

	Instances instances_;

};

} // anonymous namespace

ResourceView::ResourceView(const graphics::Buffer& buffer, graphics::PixelFormat elementFormat) :
	resourceViewId_(ResourceViewFactory::instance()->create(buffer, elementFormat))
{
}

ResourceView::ResourceView(const graphics::Texture& texture) :
	resourceViewId_(ResourceViewFactory::instance()->create(texture))
{
}

void ResourceView::bind(graphics::CommandList& commandList, graphics::ShaderType stage, size_t slot) const {
	if (resourceViewId_ == INVALID_RESOURCE_VIEW_ID) {
		commandList.setResource(graphics::ResourceView(), stage, slot);
	} else {
		commandList.setResource(ResourceViewFactory::instance()->get(resourceViewId_), stage, slot);
	}
}
