#ifndef DORMOUSEENGINE_LOGGER_APPENDER_DEBUGWINDOWAPPENDER_HPP_
#define DORMOUSEENGINE_LOGGER_APPENDER_DEBUGWINDOWAPPENDER_HPP_

#include "dormouse-engine/system/platform.hpp"

#if defined(DE_COMPILER_VISUAL_CXX)

#include "Appender.hpp"

#include <string>

#include "dormouse-engine/essentials/policy/creation/New.hpp"

namespace dormouse_engine::logger::appender {

class DebugWindowAppender : public Appender {
public:

	static const std::string CLASS_NAME;

	DebugWindowAppender(Level level, layout::LayoutSharedPtr layout) :
		Appender(level, layout)
	{
	}

	DebugWindowAppender(
		const Id& id,
		const logger::configuration::LoggerConfiguration& configuration,
		layout::LayoutFactory* layoutFactory
		) :
		Appender(id, configuration, layoutFactory)
	{
	}

protected:

	void doAppend(const std::string& message) override;

private:

	DebugWindowAppender() {
	}

	friend class essentials::Initialiser<
		Appender,
		const Id&,
		const logger::configuration::LoggerConfiguration&,
		layout::LayoutFactory*
		>;

};

} // namespace dormouse_engine::logger::appender

#endif /* DE_COMPILER_VISUAL_CXX */

#endif /* DORMOUSEENGINE_LOGGER_APPENDER_DEBUGWINDOWAPPENDER_HPP_ */
