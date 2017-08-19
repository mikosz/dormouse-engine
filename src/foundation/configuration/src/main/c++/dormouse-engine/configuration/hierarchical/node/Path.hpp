#ifndef DORMOUSEENGINE_CONFIGURATION_HIERARCHICAL_NODE_PATH_HPP_
#define DORMOUSEENGINE_CONFIGURATION_HIERARCHICAL_NODE_PATH_HPP_

#include <string>
#include <deque>
#include <iosfwd>

#include <boost/operators.hpp>

#include "dormouse-engine/configuration/configuration-exceptions.hpp"

#include "Selector.hpp"

namespace dormouse_engine {
namespace configuration {
namespace hierarchical {

class HierarchicalConfiguration;

namespace node {

class NonEmptyPathExpected : public ConfigurationException {
public:

	NonEmptyPathExpected(const std::string& operation);

	~NonEmptyPathExpected() throw () {
	}

	const std::string& operation() const {
		return operation_;
	}

private:

	static std::string constructMessage(const std::string& operation);

	std::string operation_;

};

class Path :
	public boost::dividable<Path>,
	public boost::equality_comparable<Path>
{
public:

	struct Element {

		typedef std::string Name;

		typedef std::vector<ConstSelectorSharedPtr> Selectors;

		Name name;

		Selectors selectors;

		Element(const std::string& name, ConstSelectorSharedPtr selector = ConstSelectorSharedPtr()) :
			name(name)
		{
			if (selector) {
				selectors.push_back(selector);
			}
		}

		bool operator==(const Element& other) const;

		bool selectorsMatch(const HierarchicalConfiguration& configurationNode) const;

		bool empty() const;

		std::string string() const;

	};

	static const char SEPARATOR = '/';

	Path(const Element& element);

	Path(ConstSelectorSharedPtr selector = ConstSelectorSharedPtr());

	Path(const std::string& path, ConstSelectorSharedPtr selector = ConstSelectorSharedPtr());

	Path(const char* path, ConstSelectorSharedPtr selector = ConstSelectorSharedPtr());

	bool operator==(const Path& other) const;

	Path& operator/=(const Path& other);

	Path operator[](const Path& subPath) const;

	Path is(const std::string& text) const;

	Path root() const;

	Path parentPath() const;

	Path childPath() const;

	const Element& child() const;

	bool empty() const;

	std::string string() const;

private:

	typedef std::deque<Element> Elements;

	Elements path_;

	void parse_(const std::string& string, ConstSelectorSharedPtr selector, Elements* pathParam);

};

std::ostream& operator<<(std::ostream& os, const Path::Element& pathElement);
std::ostream& operator<<(std::ostream& os, const Path& path);

} // namespace node
} // namespace hierarchical
} // namespace configuration
} // namespace dormouse_engine

namespace std {

template <>
struct hash<dormouse_engine::configuration::hierarchical::node::Path> {

	size_t operator()(const dormouse_engine::configuration::hierarchical::node::Path& nodePath) const {
		return std::hash<std::string>()(nodePath.string());
	}

};

} // namespace std

#endif /* DORMOUSEENGINE_CONFIGURATION_HIERARCHICAL_NODE_PATH_HPP_ */
