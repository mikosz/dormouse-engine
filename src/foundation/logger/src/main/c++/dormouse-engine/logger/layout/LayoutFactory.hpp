#ifndef DORMOUSEENGINE_LOGGER_LAYOUT_LAYOUTFACTORY_HPP_
#define DORMOUSEENGINE_LOGGER_LAYOUT_LAYOUTFACTORY_HPP_

#include <string>
#include <functional>

#include "Layout.hpp"

#include "dormouse-engine/essentials/policy/creation/Functor.hpp"
#include "dormouse-engine/essentials/policy/creation/New.hpp"

#include "dormouse-engine/factory.hpp"

#include "dormouse-engine/essentials/Null.hpp"

#include "dormouse-engine/logger/configuration/LoggerConfiguration.hpp"

namespace dormouse_engine::logger::layout {

class LayoutFactory :
	public Factory<
		std::string,
		factory::CreatorRegistry<std::string, policy::creation::New<Layout>, factory::error_policy::ExceptionThrowing>,
		factory::storage::Permanent
		>
{
public:

	using LayoutTypeId = std::string;

	LayoutFactory(configuration::ConstLoggerConfigurationSharedPtr loggerConfiguration);

	template <class ConcreteLayoutType>
	essentials::Null registerType(const LayoutTypeId& layoutTypeId) {
		typeFactory_.registerCreator(
			layoutTypeId,
			policy::creation::Functor<FunctorType>(
				[]() {
					return std::unique_ptr<Layout::Initialiser>(
						new Layout::Initialiser(Layout::Initialiser::createInitialisable<ConcreteLayoutType>())
						);
				}
				)
			);

		return essentials::Null::null;
	}

	void reloadConfiguration(logger::configuration::ConstLoggerConfigurationSharedPtr loggerConfiguration);

	LayoutSharedPtr create(const Layout::Id& layoutId);

private:

	using FunctorType = std::function<std::unique_ptr<Layout::Initialiser>()>;

	using LayoutTypeFactory = Factory<
		LayoutTypeId,
		factory::CreatorRegistry<
			LayoutTypeId,
			policy::creation::Functor<FunctorType>,
			factory::error_policy::ExceptionThrowing
			>,
		factory::storage::None
		>;

	LayoutTypeFactory typeFactory_;

	factory::storage::Permanent<Layout::Id, std::unique_ptr<Layout>> instanceStorage_;

	configuration::ConstLoggerConfigurationSharedPtr loggerConfiguration_;

};

} // namespace dormouse_engine::logger::layout

#endif /* DORMOUSEENGINE_LOGGER_LAYOUT_LAYOUTFACTORY_HPP_ */
