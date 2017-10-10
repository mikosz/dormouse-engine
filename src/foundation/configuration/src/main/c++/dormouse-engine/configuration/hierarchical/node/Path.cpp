#include "configuration.pch.hpp"

#include "Path.hpp"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <functional>
#include <iostream>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "dormouse-engine/essentials/pointee.hpp"

#include "SelectorIs.hpp"
#include "SelectorHas.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::configuration;
using namespace dormouse_engine::configuration::hierarchical;
using namespace dormouse_engine::configuration::hierarchical::node;

NonEmptyPathExpected::NonEmptyPathExpected(const std::string& operation) :
	ConfigurationException(constructMessage(operation)),
	operation_(operation)
{
}

std::string NonEmptyPathExpected::constructMessage(const std::string& operation) {
	std::ostringstream oss;
	oss << "Empty node path provided where a non-empty path expected while calling \""
			<< operation << "\"";
	return oss.str();
}

bool Path::Element::selectorsMatch(const HierarchicalConfiguration& configurationNode) const {
	return std::find_if_not(
		selectors.begin(),
		selectors.end(),
		[&](ConstSelectorSharedPtr selector) { return selector->matches(configurationNode); }
		) == selectors.end();
}

bool Path::Element::empty() const {
	return name.empty() && selectors.empty();
}

bool Path::Element::operator==(const Element& other) const {
	if (name != other.name) {
		return false;
	}

	if (selectors.size() != other.selectors.size()) {
		return false;
	}

	return std::equal(
		selectors.begin(),
		selectors.end(),
		other.selectors.begin(),
		[](ConstSelectorSharedPtr lhs, ConstSelectorSharedPtr rhs) { return *lhs == *rhs; }
		);
}

std::string Path::Element::string() const {
	std::ostringstream oss;
	oss << name;
	for (auto selector : selectors) {
		oss << '[' << *selector << ']';
	}
	return oss.str();
}

Path::Path(const Element& element) {
	if (!element.empty()) {
		path_.push_back(element);
	}
}

Path::Path(ConstSelectorSharedPtr selector) { // can't use initializer list here?
	if (selector) {
		Element element("", selector);
		path_.push_back(element);
	}
}

Path::Path(const std::string& path, ConstSelectorSharedPtr selector) {
	parse_(path, selector, &path_);
}

Path::Path(const char* path, ConstSelectorSharedPtr selector) {
	parse_(path, selector, &path_);
}

bool Path::operator==(const Path& other) const {
	return path_ == other.path_;
}

Path& Path::operator/=(const Path& other) {
	if (other.empty()) {
		return *this;
	}

	auto otherBegin = other.path_.begin();

	if (otherBegin->name.empty()) {
		assert(!otherBegin->selectors.empty());
		if (path_.empty()) {
			path_.push_back(Element(""));
		}
		std::copy(
			otherBegin->selectors.begin(),
			otherBegin->selectors.end(),
			std::back_inserter(path_.back().selectors)
			);
		++otherBegin;
	}

	std::copy(otherBegin, other.path_.end(), std::back_inserter(path_));
	path_.erase(
		std::remove_if(
			path_.begin(),
			path_.end(),
			[](const Element& element) { return element.name.empty() && element.selectors.empty(); }
			),
		path_.end()
		);
	return *this;
}

Path Path::operator[](const Path& subPath) const {
	Path result(*this);
	if (result.path_.empty()) {
		result.path_.push_back(Element(""));
	}

	result.path_.back().selectors.push_back(std::make_shared<SelectorHas>(subPath));

	return result;
}

Path Path::is(const std::string& text) const {
	Path result(*this);
	if (result.path_.empty()) {
		result.path_.push_back(Element(""));
	}

	result.path_.back().selectors.push_back(std::make_shared<SelectorIs>(text));

	return result;
}

Path Path::root() const {
	if (path_.empty()) {
		throw NonEmptyPathExpected("root");
	}

	return path_.front();
}

Path Path::parentPath() const {
	if (path_.empty()) {
		throw NonEmptyPathExpected("parentPath");
	}
	Path p(*this);
	p.path_.pop_back();
	return p;
}

Path Path::childPath() const {
	if (path_.empty()) {
		throw NonEmptyPathExpected("childPath");
	}
	Path c(*this);
	c.path_.pop_front();
	return c;
}

const Path::Element& Path::child() const {
	if (path_.empty()) {
		throw NonEmptyPathExpected("child");
	}
	return path_.back();
}

bool Path::empty() const {
	return path_.empty();
}

std::string Path::string() const {
	std::ostringstream oss; // TODO: boost::join compilation issues here, but I want it!
	bool first = true;
	for (auto element : path_) {
		if (first) {
			first = false;
		} else {
			oss << '/';
		}
		oss << element;
	}
	return oss.str();
}

void Path::parse_(const std::string& pathString, ConstSelectorSharedPtr selector, Elements* pathParam) {
	Elements& path = essentials::pointee(pathParam);

	std::vector<std::string> pathNames;
	boost::split(pathNames, pathString, boost::is_any_of(std::string() + Path::SEPARATOR));
	pathNames.erase(
		std::remove_if(
			pathNames.begin(),
			pathNames.end(), 
			[](const Element& e) { return e.name.empty(); }
			),
		pathNames.end()
		);

	std::copy(pathNames.begin(), pathNames.end(), std::back_inserter(path));

	if (selector) {
		if (path_.empty()) {
			path_.push_back(Element(""));
		}
		path_.back().selectors.push_back(selector);
	}
}

std::ostream& dormouse_engine::configuration::hierarchical::node::operator<<(
	std::ostream& os, const Path::Element& pathElement) {
	return os << pathElement.string();
}

std::ostream& dormouse_engine::configuration::hierarchical::node::operator<<(
	std::ostream& os, const Path& path) {
	return os << path.string();
}
