#ifndef DORMOUSEENGINE_LOGGER_LAYOUT_EMPTYLAYOUT_HPP_
#define DORMOUSEENGINE_LOGGER_LAYOUT_EMPTYLAYOUT_HPP_

#include "Layout.hpp"

namespace dormouse_engine {
namespace logger {
namespace layout {

class EmptyLayout : public Layout {
public:

	static const std::string CLASS_NAME;

	EmptyLayout() {
	}

	std::string format(Level /* level */, const Context& /* context */, const std::string& message) {
		return message + '\n';
	}

private:

	friend class essentials::Initialiser<
		Layout,
		const Id&,
		const logger::configuration::LoggerConfiguration&
	>;

};

} // namespace layout
} // namespace logger
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_LOGGER_LAYOUT_EMPTYLAYOUT_HPP_ */
