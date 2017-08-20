#include "../platform.hpp"

#if defined DE_COMPILER_VISUAL_CXX

#	if defined(min)
#		undef min
#	endif /* min */

#	if defined(max)
#		undef max
#	endif /* max */

#	if defined(ERROR)
#		undef ERROR
#	endif /* ERROR */

#	if defined(OPAQUE)
#		undef OPAQUE
#	endif /* OPAQUE */

#	if defined(TRANSPARENT)
#		undef TRANSPARENT
#	endif /* TRANSPARENT */

#	if defined(DOMAIN)
#		undef DOMAIN
#	endif /* DOMAIN */

#endif /* DE_COMPILER_VISUAL_CXX */
