#ifndef DORMOUSEENGINE_LOGGER_LAYOUT_BASICLAYOUT_HPP_
#define DORMOUSEENGINE_LOGGER_LAYOUT_BASICLAYOUT_HPP_

#include "Layout.hpp"

#include <sstream>

namespace dormouse_engine {
namespace logger {
namespace layout {

class BasicLayout : public Layout {
public:

	static const std::string CLASS_NAME;

	BasicLayout() {
	}

	std::string format(Level level, const Context& context, const std::string& message) {
		std::ostringstream oss;
		oss << context.category << " [" << level << "] - " << message << '\n';
		return oss.str();
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

#endif /* DORMOUSEENGINE_LOGGER_LAYOUT_BASICLAYOUT_HPP_ */
