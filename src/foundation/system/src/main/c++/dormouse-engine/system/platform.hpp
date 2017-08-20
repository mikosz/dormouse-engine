#ifndef DORMOUSEENGINE_SYSTEM_PLATFORM_HPP_
#define DORMOUSEENGINE_SYSTEM_PLATFORM_HPP_

namespace dormouse_engine::system {

enum class Platform {
	WINDOWS,
	LINUX,
};

#if defined(_WIN32)
#	define DE_PLATFORM_WINDOWS
const Platform PLATFORM = Platform::WINDOWS;
#elif defined(__linux__)
#	define DE_PLATFORM_LINUX
const Platform PLATFORM = Platform::LINUX;
#else
#	error "Failed to discover the system platform"
#endif

enum class Compiler {
	VISUAL_CXX,
	GCC,
	CLANG,
};

#if defined(_MSC_VER)
#	define DE_COMPILER_VISUAL_CXX
const Compiler COMPILER = Compiler::VISUAL_CXX;
#elif defined(__GCC__)
#	define DE_COMPILER_GCC
const Compiler COMPILER = Compiler::GCC;
#elif defined(__clang__)
#	define DE_COMPILER_CLANG
const Compiler COMPILER = Compiler::CLANG;
#endif

} // namespace dormouse_engine::system

#endif /* DORMOUSEENGINE_SYSTEM_PLATFORM_HPP_ */
