#include "Logging.hpp"

#include "dormouse-engine/logger.hpp"


DE_LOGGER_CATEGORY("COCONUT_TOOLS.FACTORY.ERROR_POLICY");

namespace dormouse_engine {
namespace factory {
namespace error_policy {

void log_(const std::string& message) {
	DE_LOG_ERROR << message;
}

}  // namespace error_policy
}  // namespace factory
}  // namespace dormouse_engine
