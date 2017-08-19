#include "HierarchicalConfigurationReader.hpp"

#include <fstream>
#include <stack>
#include <functional>

#include "dormouse-engine/configuration/hierarchical/HierarchicalConfiguration.hpp"
#include "dormouse-engine/essentials/pointee.hpp"
#include "dormouse-engine/exceptions/LogicError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::configuration;
using namespace dormouse_engine::configuration::readers;

namespace {

class UnpairedParserBeginEnds : public exceptions::LogicError {
public:

	UnpairedParserBeginEnds() :
		exceptions::LogicError("Expected paired newChild/childrenEnd calls from parser") {
	}

};

struct RootsEntry {

	std::string node;

	hierarchical::HierarchicalConfigurationSharedPtr subtree;

	RootsEntry(const std::string& node, hierarchical::HierarchicalConfigurationSharedPtr subtree) :
		node(node),
		subtree(subtree)
	{
	}

};

typedef std::stack<RootsEntry> Roots;

void newChild(Roots* rootsPtr, const std::string& node, const std::string& text) {
	Roots& roots = essentials::pointee(rootsPtr);
	roots.push(RootsEntry(node, hierarchical::HierarchicalConfiguration::create(text)));
}

void childrenEnd(Roots* rootsPtr) {
	Roots& roots = essentials::pointee(rootsPtr);
	RootsEntry child = roots.top();
	roots.pop();
	if (roots.empty()) {
		throw UnpairedParserBeginEnds();
	}
	roots.top().subtree->add(child.node, child.subtree);
}

} // anonymous namespace

void HierarchicalConfigurationReader::read(
		const parsers::HierarchicalParser& parser,
		std::istream& is,
		hierarchical::HierarchicalConfiguration* configurationPtr
		) const {
	hierarchical::HierarchicalConfiguration& configuration = essentials::pointee(configurationPtr);

	Roots roots;
	roots.push(RootsEntry(std::string(), configuration.shared_from_this()));

	parser.parse(
			is,
			std::bind(&newChild, &roots, std::placeholders::_1, std::placeholders::_2),
			std::bind(&childrenEnd, &roots)
			);

	if (roots.size() != 1) {
		throw UnpairedParserBeginEnds();
	}

	roots.pop();
}

void HierarchicalConfigurationReader::read(
		const parsers::HierarchicalParser& parser,
		const boost::filesystem::path& path,
		hierarchical::HierarchicalConfiguration* configurationPtr
		) const {
	hierarchical::HierarchicalConfiguration& configuration = essentials::pointee(configurationPtr);

	Roots roots;
	roots.push(RootsEntry(std::string(), configuration.shared_from_this()));

	parser.parse(
			path,
			std::bind(&newChild, &roots, std::placeholders::_1, std::placeholders::_2),
			std::bind(&childrenEnd, &roots)
			);

	if (roots.size() != 1) {
		throw UnpairedParserBeginEnds();
	}

	roots.pop();
}
