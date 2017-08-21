#include "Angle.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine;
using namespace dormouse_engine::math;

const Angle Angle::RIGHT(::radians(PI / 2.0f));
const Angle Angle::HALF_FULL(PI);
const Angle Angle::FULL(::radians(2.0f * PI));

std::ostream& dormouse_engine::math::operator<<(std::ostream& os, const Angle& angle) {
	return os << angle.radians();
}
