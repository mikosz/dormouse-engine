#ifndef _DORMOUSEENGINE_GRAPHICS_DX11_COMPILESHADER_HPP_
#define _DORMOUSEENGINE_GRAPHICS_DX11_COMPILESHADER_HPP_

#include <d3dcommon.h>

#include <functional>
#include <vector>
#include <cstddef>
#include <memory>
#include <iosfwd>
#include <string>

#include <d3dcompiler.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/enums/Mask.hpp"
#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "ShaderType.hpp"

namespace dormouse_engine::graphics {

class ShaderCompiler {
public:

    using IncludeHandler = std::function<std::shared_ptr<essentials::ByteVector>(const std::string&)>;

    DE_MEMBER_FLAG_VALUES(
        CompilerFlag,
        (DEBUG)(D3DCOMPILE_DEBUG)
        (SKIP_OPTIMISATION)(D3DCOMPILE_SKIP_OPTIMIZATION)
		(OPTIMISATION_LEVEL_0)(D3DCOMPILE_OPTIMIZATION_LEVEL0)
		(OPTIMISATION_LEVEL_1)(D3DCOMPILE_OPTIMIZATION_LEVEL1)
		(OPTIMISATION_LEVEL_2)(D3DCOMPILE_OPTIMIZATION_LEVEL2)
		(OPTIMISATION_LEVEL_3)(D3DCOMPILE_OPTIMIZATION_LEVEL3)
        );

    using CompilerFlags = dormouse_engine::Mask<CompilerFlag>;

	static const CompilerFlags FULL_DEBUG_MASK;

    ShaderCompiler(CompilerFlags globalFlags = CompilerFlags()) :
        globalCompilerFlags_(globalFlags)
    {
    }

	essentials::ByteVector compile(
		essentials::ConstBufferView code,
		const std::string& name,
	    const std::string& entrypoint,
    	ShaderType type,
        IncludeHandler includeHandler = IncludeHandler(),
        CompilerFlags instanceFlags = CompilerFlags()
        ) const;

private:

    CompilerFlags globalCompilerFlags_;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_DX11_COMPILESHADER_HPP_ */
