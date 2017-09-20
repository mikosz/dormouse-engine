#include "graphics.pch.hpp"

#include "ShaderCompiler.hpp"

#include <algorithm>
#include <cstdlib>

#include "DirectXError.hpp"

#include "dormouse-engine/logger.hpp"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

using namespace std::string_literals;

namespace /* anonymous */ {

DE_LOGGER_CATEGORY("DORMOUSE_ENGINE.GRAPHICS.COMPILE_SHADER");

std::string featureLevel(ShaderType shaderType) {
	switch (shaderType) {
	case ShaderType::VERTEX:
		return "vs_5_0"s;
	case ShaderType::GEOMETRY:
		return "gs_5_0"s;
	case ShaderType::HULL:
		return "hs_5_0"s;
	case ShaderType::DOMAIN:
		return "ds_5_0"s;
	case ShaderType::PIXEL:
		return "ps_5_0"s;
	default:
		assert(!"Unsupported shader type"); // TODO: should throw
		return ""s;
	}
}

class Includer : public ID3DInclude {
public:

	Includer(ShaderCompiler::IncludeHandler handler) :
		handler_(std::move(handler))
	{
	}

	HRESULT Open(
		D3D_INCLUDE_TYPE /*includeType*/,
		LPCSTR fileName,
		LPCVOID /*parentData*/,
		LPCVOID* data,
		UINT* bytes
		) override
	{
		if (handler_) {
			data_.emplace_back(handler_(fileName));
			*data = data_.back()->data();
			*bytes = static_cast<UINT>(data_.back()->size());
		}
		return S_OK;
	}

	HRESULT Close(LPCVOID /*data*/) override {
		return S_OK;
	}

private:

	ShaderCompiler::IncludeHandler handler_;

	std::vector<std::shared_ptr<essentials::ByteVector>> data_;

};

} // anonymous namespace

const ShaderCompiler::CompilerFlags ShaderCompiler::FULL_DEBUG_MASK =
	CompilerFlags() |
	CompilerFlag::DEBUG |
	CompilerFlag::SKIP_OPTIMISATION |
	CompilerFlag::OPTIMISATION_LEVEL_0
	;

essentials::ByteVector ShaderCompiler::compile(
	essentials::ConstBufferView code,
    const std::string& name,
	const std::string& entrypoint,
    ShaderType type,
    IncludeHandler includeHandler,
    CompilerFlags instanceFlags
    ) const
{
	auto includer = Includer(includeHandler);

	system::windows::COMWrapper<ID3DBlob> codeBlob;
	system::windows::COMWrapper<ID3DBlob> errors;
	auto result = D3DCompile(
		code.data(),
		code.size(),
		name.c_str(),
		nullptr,
		&includer,
		entrypoint.c_str(),
		featureLevel(type).c_str(),
		(globalCompilerFlags_ | instanceFlags).integralValue(),
		0,
		&codeBlob.get(),
		&errors.get()
		);

	if (errors) {
		dormouse_engine::logger::Level logLevel;
		std::string prefix;

		if (FAILED(result)) {
			prefix = "HLSL compiler error";
			logLevel = dormouse_engine::logger::Level::CRITICAL;
		} else {
			prefix = "HLSL compiler warning";
			logLevel = dormouse_engine::logger::Level::WARNING;
		}

		DE_LOG(logLevel) << prefix << ": " << reinterpret_cast<const char*>(errors->GetBufferPointer());
	}

	if (FAILED(result)) {
		throw DirectXError(result, "Failed to compile a shader");
	}

	auto data = essentials::ByteVector();
	data.reserve(codeBlob->GetBufferSize());
	std::copy(
		reinterpret_cast<const std::uint8_t*>(codeBlob->GetBufferPointer()),
		reinterpret_cast<const std::uint8_t*>(codeBlob->GetBufferPointer()) + codeBlob->GetBufferSize(),
		std::back_inserter(data)
		);

	return data;
}
