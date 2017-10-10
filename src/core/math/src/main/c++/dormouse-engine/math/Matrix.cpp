#include "Matrix.hpp"

#pragma warning(push, 3)
#	include <ponder/classbuilder.hpp>
#pragma warning(pop)

#include "dormouse-engine/reflection/Object.hpp"
#include "dormouse-engine/exceptions/RuntimeError.hpp"
#include "dormouse-engine/graphics/Device.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::math;

void Matrix4x4::writeShaderData(essentials::BufferView buffer, graphics::ShaderDataType dataType) const {
	if (dataType.scalarType != graphics::ShaderDataType::ScalarType::FLOAT) {
		throw exceptions::RuntimeError("Matrices are not writeable to scalar type " + toString(dataType.scalarType));
	}

	auto needsTranspose = false;

	if (dataType.klass == graphics::ShaderDataType::Class::MATRIX_COLUMN_MAJOR) {
		needsTranspose = math::Matrix4x4::IS_ROW_MAJOR;
	} else if (dataType.klass == graphics::ShaderDataType::Class::MATRIX_ROW_MAJOR) {
		needsTranspose = math::Matrix4x4::IS_COLUMN_MAJOR;
	} else {
		throw exceptions::RuntimeError("Matrices are not writeable to class " + toString(dataType.klass));
	}

	if constexpr (math::Matrix4x4::VECTOR_IS_SINGLE_ROW_MATRIX != graphics::Device::VECTOR_IS_SINGLE_ROW_MATRIX) {
		needsTranspose = !needsTranspose;
	}

	auto* target = reinterpret_cast<math::Matrix4x4*>(buffer.data());

	if (needsTranspose) {
		*target = ::transpose(view());
	} else {
		*target = *this;
	}
}

void detail::declareMatrix4x4() {
	ponder::Class::declare<Matrix4x4>("dormouse_engine::math::Matrix4x4")
		.tag(reflection::ClassTag::SHADER_DATA)
		.function("writeShaderData", &Matrix4x4::writeShaderData).tag(reflection::FunctionTag::WRITE_SHADER_DATA)
		;
}
