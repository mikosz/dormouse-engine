#ifndef DORMOUSEENGINE_APPENDER_APPENDERFACTORY_HPP_
#define DORMOUSEENGINE_APPENDER_APPENDERFACTORY_HPP_

#include <functional>

#include "Appender.hpp"

#include "dormouse-engine/factory.hpp"
#include "dormouse-engine/logger/configuration/LoggerConfiguration.hpp"
#include "dormouse-engine/logger/layout/LayoutFactory.hpp"

namespace dormouse_engine::logger::appender {

class AppenderFactory {
public:

	typedef std::string AppenderTypeId;

	AppenderFactory(configuration::ConstLoggerConfigurationSharedPtr loggerConfiguration);

	template <class ConcreteAppenderType>
	void registerType(const AppenderTypeId& appenderTypeId) {
		typeFactory_.registerCreator(
			appenderTypeId,
			policy::creation::Functor<FunctorType>(
				[]() {
					return std::unique_ptr<Appender::Initialiser>(
							new Appender::Initialiser(Appender::Initialiser::createInitialisable<ConcreteAppenderType>())
							);
				}
				)
			);
	}

	void reloadConfiguration(logger::configuration::ConstLoggerConfigurationSharedPtr loggerConfiguration);

	AppenderSharedPtr create(const Appender::Id& appenderId);

	layout::LayoutFactory& layoutFactory() {
		return layoutFactory_;
	}

private:

	using FunctorType = std::function<std::unique_ptr<Appender::Initialiser>()>;

	using AppenderTypeFactory = Factory<
		AppenderTypeId,
		factory::CreatorRegistry<AppenderTypeId, policy::creation::Functor<FunctorType>, factory::error_policy::ExceptionThrowing>,
		factory::storage::None
		>;

	AppenderTypeFactory typeFactory_;

	factory::storage::Permanent<Appender::Id, std::unique_ptr<Appender>> instanceStorage_;

	configuration::ConstLoggerConfigurationSharedPtr loggerConfiguration_;

	layout::LayoutFactory layoutFactory_;

};

} // namespace dormouse_engine::logger::appender

#endif /* DORMOUSEENGINE_APPENDER_APPENDERFACTORY_HPP_ */
