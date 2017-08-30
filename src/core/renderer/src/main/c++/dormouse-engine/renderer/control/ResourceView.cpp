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
	size_t create(
		graphics::Device& graphicsDevice,
		const graphics::ResourceView::Configuration& configuration
		)
	{
		auto it = index_.find(configuration);

		if (it == index_.end()) {
			it = index_.emplace_hint(it, configuration, instances_.size());
			instances_.emplace_back(graphicsDevice, configuration);
		}

		return it->second;
	}

	graphics::ResourceView get(size_t id) const {
		assert(id < instances_.size());
		return instances_[id];
	}

private:

	using Index = std::unordered_map<
		graphics::ResourceView::Configuration,
		size_t,
		ConfigurationHash,
		ConfigurationEqual
		>;

	using Instances = std::vector<graphics::ResourceView>;

	Index index_;

	Instances instances_;

};

} // anonymous namespace

ResourceView::ResourceView(graphics::Resource& resource) :
	resourceViewId_(ResourceViewFactory::instance()->create(resource))
{
}

void ResourceView::bind(graphics::CommandList& commandList, graphics::ShaderType stage, size_t slot) const {
	if (resourceViewId_ == INVALID_RESOURCE_VIEW_ID) {
		commandList.setResource(graphics::ResourceView(), stage, slot);
	} else {
		commandList.setResource(ResourceViewFactory::instance()->get(resourceViewId_), stage, slot);
	}
}
