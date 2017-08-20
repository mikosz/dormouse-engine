#ifndef DORMOUSEENGINE_ESSENTIALS_DEBUG_HPP_
#define DORMOUSEENGINE_ESSENTIALS_DEBUG_HPP_

namespace dormouse_engine::essentials {

#if defined(NDEBUG)
const auto IS_DEBUG = false;
#else
const auto IS_DEBUG = true;
#endif /* NDEBUG */

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_ESSENTIALS_DEBUG_HPP_ */
