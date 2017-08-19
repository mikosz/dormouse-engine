#ifndef _DORMOUSEENGINE_CONFIGURATION_HIERARCHICAL_NODE_SELECTORHAS_HPP_
#define _DORMOUSEENGINE_CONFIGURATION_HIERARCHICAL_NODE_SELECTORHAS_HPP_

#include <string>

#include "Selector.hpp"
#include "Path.hpp"

namespace dormouse_engine {
namespace configuration {
namespace hierarchical {
namespace node {

class SelectorHas : public Selector {
public:

	SelectorHas(const Path& nodePath) :
		nodePath_(nodePath)
	{
	}

	bool matches(const HierarchicalConfiguration& node) const override;

	std::string string() const override;

protected:

	Type selectorType() const override {
		return HAS;
	}

	bool equals(const Selector& other) const override {
		return nodePath_ == dynamic_cast<const SelectorHas&>(other).nodePath_;
	}

private:

	Path nodePath_;

};

} // namespace node
} // namespace hierarchical
} // namespace configuration
} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_CONFIGURATION_HIERARCHICAL_NODE_SELECTORHAS_HPP_ */
