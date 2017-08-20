#ifndef _DORMOUSEENGINE_CONFIGURATION_HIERARCHICAL_NODE_SELECTORIS_HPP_
#define _DORMOUSEENGINE_CONFIGURATION_HIERARCHICAL_NODE_SELECTORIS_HPP_

#include <string>

#include "Selector.hpp"

namespace dormouse_engine::configuration::hierarchical::node {

class SelectorIs : public Selector {
public:

	SelectorIs(const std::string& nodeText) :
		nodeText_(nodeText)
	{
	}

	bool matches(const HierarchicalConfiguration& node) const override;

	std::string string() const override;

protected:

	Type selectorType() const override {
		return IS;
	}

	bool equals(const Selector& other) const override {
		return nodeText_ == dynamic_cast<const SelectorIs&>(other).nodeText_;
	}

private:

	std::string nodeText_;

};

} // namespace dormouse_engine::configuration::hierarchical::node

#endif /* _DORMOUSEENGINE_CONFIGURATION_HIERARCHICAL_NODE_SELECTORIS_HPP_ */
