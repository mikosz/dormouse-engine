#ifndef _DORMOUSEENGINE_CONFIGURATION_HIERARCHICAL_NODE_SELECTOR_HPP_
#define _DORMOUSEENGINE_CONFIGURATION_HIERARCHICAL_NODE_SELECTOR_HPP_

#include <iosfwd>
#include <string>

#include <boost/noncopyable.hpp>
#include <boost/operators.hpp>

#include "dormouse-engine/essentials/smart-pointer-definitions.hpp"

namespace dormouse_engine {
namespace configuration {
namespace hierarchical {

class HierarchicalConfiguration;

namespace node {

class Selector :
	boost::equality_comparable<Selector>,
	boost::noncopyable
{
public:

	virtual ~Selector() {
	}

	bool operator==(const Selector& other) const {
		return selectorType() == other.selectorType() && equals(other);
	}

	virtual bool matches(const HierarchicalConfiguration& node) const = 0;

	virtual std::string string() const = 0;

protected:

	enum Type {
		IS,
		HAS,
	};

	Selector() {
	}

	virtual Type selectorType() const = 0;

	virtual bool equals(const Selector& other) const = 0;

};

std::ostream& operator<<(std::ostream& os, const Selector& selector);


DE_SMART_POINTER_DEFINITONS(Selector);

} // namespace node
} // namespace hierarchical
} // namespace configuration
} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_CONFIGURATION_HIERARCHICAL_NODE_SELECTOR_HPP_ */
