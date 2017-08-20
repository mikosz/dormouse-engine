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

    using ShaderData = const std::vector<std::uint8_t>;
    using IncludeHandler = std::function<std::shared_ptr<ShaderData>(const std::string&)>;

    DE_MEMBER_FLAG_VALUES(
        CompilerFlag,
        (DEBUG)(D3DCOMPILE_DEBUG)
        (SKIP_OPTIMISATION)(D3DCOMPILE_SKIP_OPTIMIZATION)
        );

    using CompilerFlags = dormouse_engine::Mask<CompilerFlag>;

    ShaderCompiler(CompilerFlags globalFlags = CompilerFlags()) :
        globalCompilerFlags_(globalFlags)
    {
    }

    ShaderData compile(
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
