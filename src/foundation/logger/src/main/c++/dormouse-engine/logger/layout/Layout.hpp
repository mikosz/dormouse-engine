#ifndef DORMOUSEENGINE_LOGGER_LAYOUT_LAYOUT_HPP_
#define DORMOUSEENGINE_LOGGER_LAYOUT_LAYOUT_HPP_

#include <string>
#include <memory>

#include "dormouse-engine/logger/configuration/LoggerConfiguration.hpp"

#include "dormouse-engine/essentials/smart-pointer-definitions.hpp"
#include "dormouse-engine/essentials/Initialisable.hpp"

#include "../Context.hpp"

namespace dormouse_engine {
namespace logger {
namespace layout {

class Layout :
	public essentials::Initialisable<
		Layout,
		const std::string&,
		const logger::configuration::LoggerConfiguration&
	>
{
public:

	typedef std::string Id;

	virtual ~Layout() {
	}

	virtual std::string format(Level level, const Context& context, const std::string& message) = 0;

protected:

	Layout() {
	}

	void doInitialise(
		const Id& /*id*/,
		const logger::configuration::LoggerConfiguration& /*configuration*/
		) override {
	}

};

DE_SMART_POINTER_DEFINITONS(Layout);

} // namespace layout
} // namespace logger
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_LOGGER_LAYOUT_LAYOUT_HPP_ */
