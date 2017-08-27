#ifndef DORMOUSEENGINE_GRAPHICS_GRAPHICS_PCH_HPP_
#define DORMOUSEENGINE_GRAPHICS_GRAPHICS_PCH_HPP_

#include <functional>
#include <cassert>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <array>
#include <memory>
#include <tuple>
#include <algorithm>
#include <sstream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include <boost/operators.hpp>

#include "dormouse-engine/exceptions/LogicError.hpp"
#include "dormouse-engine/exceptions/RuntimeError.hpp"
#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "dormouse-engine/essentials/types.hpp"
#include "dormouse-engine/enums.hpp"
#include "dormouse-engine/logger.hpp"

#include "dormouse-engine/graphics/PixelFormat.hpp"

#endif /* DORMOUSEENGINE_GRAPHICS_GRAPHICS_PCH_HPP_ */
