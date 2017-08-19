#ifndef DORMOUSEENGINE_LOGGER_APPENDER_APPENDER_HPP_
#define DORMOUSEENGINE_LOGGER_APPENDER_APPENDER_HPP_

#include <iosfwd>
#include <string>
#include <memory>

#include "dormouse-engine/essentials/smart-pointer-definitions.hpp"
#include "dormouse-engine/essentials/Initialisable.hpp"

#include "dormouse-engine/logger/configuration/LoggerConfiguration.hpp"
#include "dormouse-engine/logger/layout/LayoutFactory.hpp"
#include "dormouse-engine/logger/layout/Layout.hpp"
#include "dormouse-engine/logger/Context.hpp"

namespace dormouse_engine {
namespace logger {
namespace appender {

class Appender :
	public essentials::Initialisable<
		Appender,
		const std::string&,
		const logger::configuration::LoggerConfiguration&,
		layout::LayoutFactory*
		>
{
public:

	typedef std::string Id;

	virtual ~Appender() {
	}

	void append(Level level, const Context& context, const std::string& message);

protected:

	virtual void doAppend(const std::string& message) = 0;

	Appender() {
	}

	Appender(Level level, layout::LayoutSharedPtr layout) :
		level_(level),
		layout_(layout)
	{
	}

	Appender(
		const Id& id,
		const logger::configuration::LoggerConfiguration& configuration,
		layout::LayoutFactory* layoutFactory
		);

	void doInitialise(
		const Id& id,
		const logger::configuration::LoggerConfiguration& configuration,
		layout::LayoutFactory* layoutFactory
		) override;

private:

	Level level_;

	layout::LayoutSharedPtr layout_;

};


DE_SMART_POINTER_DEFINITONS(Appender);

} // namespace appender
} // namespace logger
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_LOGGER_APPENDER_APPENDER_HPP_ */
