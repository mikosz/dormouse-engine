#include "memory.hpp"

#pragma warning(push, 3)
#	include <ponder/classbuilder.hpp>
#pragma warning(pop)

using namespace dormouse_engine::essentials;

void detail::declareBufferView() {
	ponder::Class::declare<essentials::BufferView>("dormouse_engine::essentials::BufferView");
}
