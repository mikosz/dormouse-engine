#ifndef DORMOUSEENGINE_CONFIGURATION_READERS_HIERARCHICALCONFIGURATIONREADER_HPP_
#define DORMOUSEENGINE_CONFIGURATION_READERS_HIERARCHICALCONFIGURATIONREADER_HPP_

#include <boost/filesystem/path.hpp>

#include "dormouse-engine/configuration/parsers/HierarchicalParser.hpp"
#include "dormouse-engine/configuration/hierarchical/HierarchicalConfiguration.hpp"

namespace dormouse_engine {
namespace configuration {
namespace readers {

class HierarchicalConfigurationReader {
public:

	void read(
			const parsers::HierarchicalParser& parser,
			std::istream& is,
			hierarchical::HierarchicalConfiguration* configuration
			) const;

	void read(
			const parsers::HierarchicalParser& parser,
			const boost::filesystem::path& path,
			hierarchical::HierarchicalConfiguration* configuration
			) const;

};

} // namespace readers
} // namespace configuration
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_CONFIGURATION_READERS_HIERARCHICALCONFIGURATIONREADER_HPP_ */
