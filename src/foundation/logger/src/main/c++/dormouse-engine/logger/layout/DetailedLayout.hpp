#ifndef DORMOUSEENGINE_LOGGER_LAYOUT_DETAILEDLAYOUT_HPP_
#define DORMOUSEENGINE_LOGGER_LAYOUT_DETAILEDLAYOUT_HPP_

#include "dormouse-engine/exceptions/LogicError.hpp"

#include "../configuration/LoggerConfiguration.hpp"
#include "Layout.hpp"
#include "Clock.hpp"

namespace dormouse_engine::logger::layout {

class DetailedLayout : public Layout {
public:

	static const std::string CLASS_NAME;

	DetailedLayout(Clock::Precision clockPrecision) :
		clock_(clockPrecision)
	{
		if (clockPrecision == Clock::Precision::UNKNOWN) {
			throw exceptions::LogicError("Invalid clock precision");
		}
	}

	/**
	 * Formats the logged string, e.g.:
	 *   "DetailedLayout.hpp:75 @ 21.02.2015-08:31:12.125631 [WARNING] - logger-category"
	 *   "some warning message"
	 */
	std::string format(Level level, const Context& context, const std::string& message) override;

protected:

	void doInitialise(const Id& layoutId, const logger::configuration::LoggerConfiguration& configuration) override;

private:

	friend class essentials::Initialiser<
		Layout,
		const Id&,
		const logger::configuration::LoggerConfiguration&
	>;

	DetailedLayout() {
	}

	Clock clock_;

};

} // namespace dormouse_engine::logger::layout

#endif /* DORMOUSEENGINE_LOGGER_LAYOUT_DETAILEDLAYOUT_HPP_ */
