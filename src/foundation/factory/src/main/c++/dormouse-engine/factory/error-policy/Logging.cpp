#include "Logging.hpp"

#include "dormouse-engine/logger.hpp"

DE_LOGGER_CATEGORY("DORMOUSE_ENGINE.FACTORY.ERROR_POLICY");

namespace dormouse_engine::factory::error_policy {

void log_(const std::string& message) {
	DE_LOG_ERROR << message;
}

} // namespace dormouse_engine::factory::error_policy
