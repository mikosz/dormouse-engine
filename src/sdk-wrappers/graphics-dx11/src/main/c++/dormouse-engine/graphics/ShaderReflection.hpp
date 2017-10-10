#ifndef _DORMOUSEENGINE_GRAPHICS_SHADERREFLECTION_HPP_
#define _DORMOUSEENGINE_GRAPHICS_SHADERREFLECTION_HPP_

#include <string>
#include <vector>
#include <tuple>

#include <d3dcommon.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/system/windows/COMWrapper.hpp"
#include "dormouse-engine/enums.hpp"
#include "ShaderDataType.hpp"

namespace dormouse_engine::graphics {

class ShaderReflection {
public:

	struct InputParameterInfo {

		DE_MEMBER_ENUM_VALUES(
			DataType,
			(FLOAT)(D3D_REGISTER_COMPONENT_FLOAT32)
			(UINT)(D3D_REGISTER_COMPONENT_UINT32)
			(INT)(D3D_REGISTER_COMPONENT_SINT32)
			);

		std::string semantic;

		size_t semanticIndex;

		DataType dataType;

		size_t elements;

	};

	using InputParameterInfos = std::vector<InputParameterInfo>;

	struct Type {

		using Member = std::tuple<std::string, Type>;

		static const size_t MemberNameTag = 0;

		static const size_t MemberTypeTag = 1;

		using Members = std::vector<Member>;

		std::string name;

		size_t offset;

		ShaderDataType dataType;

		size_t elements;

		size_t elementOffset;

		Members members;

	};

	struct Variable {

		Type type;

		std::string name;

		size_t offset;

		size_t size;

	};

	struct ConstantBufferInfo {

		using Variables = std::vector<Variable>;

		std::string name;

		size_t size;

		size_t slot;

		Variables variables;

	};

	using ConstantBufferInfos = std::vector<ConstantBufferInfo>;

	struct ResourceInfo {

		DE_MEMBER_ENUM_VALUES(
			Type,
			(SAMPLER)(D3D_SIT_SAMPLER)
			(TEXTURE)(D3D_SIT_TEXTURE)
			);

		DE_MEMBER_ENUM_VALUES(
			Dimension,
			(UNKNOWN)(D3D_SRV_DIMENSION_UNKNOWN)
			(BUFFER)(D3D_SRV_DIMENSION_BUFFER)
			(TEXTURE1D)(D3D_SRV_DIMENSION_TEXTURE1D)
			(TEXTURE2D)(D3D_SRV_DIMENSION_TEXTURE2D)
			(TEXTURE_CUBE)(D3D_SRV_DIMENSION_TEXTURECUBE)
			);

		Type type;

		std::string name;

		size_t slot;

		Dimension dimension;

	};

	using ResourceInfos = std::vector<ResourceInfo>;

	ShaderReflection(const void* shaderData, size_t shaderSize);

	const InputParameterInfos& inputParameters() const {
		return inputParameters_;
	}

	const ConstantBufferInfos& constantBuffers() const {
		return constantBuffers_;
	}

	const ResourceInfos& resources() const {
		return resources_;
	}

private:

	InputParameterInfos inputParameters_;

	ConstantBufferInfos constantBuffers_;

	ResourceInfos resources_;

};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_SHADERREFLECTION_HPP_ */
