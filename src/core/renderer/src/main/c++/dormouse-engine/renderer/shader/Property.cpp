#include "Property.hpp"

#include <ponder/valuevisitor.hpp>

#include "../command/DrawCommand.hpp"
#include "../control/ResourceView.hpp"
#include "../control/Sampler.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;

namespace /* anonymous */ {

class ShaderPropertyVisitor : public ponder::ValueVisitor<Property> {
public:

	template <class T>
	Property operator()(const T& v) {
		return essentials::make_observer(&v);
	}

	Property operator()([[maybe_unused]] const ponder::UserObject& uo) {
		return reflection::Object(uo);
	}

};

} // anonymous namespace

bool shader::hasShaderProperty(
	reflection::Object reflectionObject,
	essentials::StringId id,
	[[maybe_unused]] size_t arrayIdx
	)
{
	const auto& metaclass = reflectionObject.metaclass();
	const auto& idString = id.string();
	
	return metaclass.hasProperty(idString);
}

Property shader::getShaderProperty(
	reflection::Object reflectionObject,
	essentials::StringId id,
	[[maybe_unused]] size_t arrayIdx
	)
{
	const auto& metaclass = reflectionObject.metaclass();

	assert(hasShaderProperty(reflectionObject, id, arrayIdx));

	const auto& property = metaclass.property(id.string());
	const auto& value = property.get(reflectionObject.metaobject());

	return value.visit(ShaderPropertyVisitor());
}

void shader::bindShaderResource(
	control::ResourceView resourceView, command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot)
{
	cmd.setResource(std::move(resourceView), stage, slot);
}

void shader::bindShaderResource(
	control::Sampler sampler, command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot)
{
	cmd.setSampler(std::move(sampler), stage, slot);
}

void shader::writeShaderData(
	const math::Matrix4x4& matrix, essentials::BufferView buffer, graphics::ShaderDataType dataType)
{
	if (dataType.scalarType != graphics::ShaderDataType::ScalarType::FLOAT) {
		throw IncompatibleDataType("Matrices are not writeable to scalar type " + toString(dataType.scalarType));
	}

	auto needsTranspose = false;

	if (dataType.klass == graphics::ShaderDataType::Class::MATRIX_COLUMN_MAJOR) {
		needsTranspose = math::Matrix4x4::IS_ROW_MAJOR;
	} else if (dataType.klass == graphics::ShaderDataType::Class::MATRIX_ROW_MAJOR) {
		needsTranspose = math::Matrix4x4::IS_COLUMN_MAJOR;
	} else {
		throw IncompatibleDataType("Matrices are not writeable to class " + toString(dataType.klass));
	}

	if constexpr (math::Matrix4x4::VECTOR_IS_SINGLE_ROW_MATRIX != graphics::Device::VECTOR_IS_SINGLE_ROW_MATRIX) {
		needsTranspose = !needsTranspose;
	}

	auto* target = reinterpret_cast<math::Matrix4x4*>(buffer.data());

	if (needsTranspose) {
		*target = transpose(matrix.view());
	} else {
		*target = matrix;
	}
}

void shader::writeShaderData(
	const math::Transform& transform, essentials::BufferView buffer, graphics::ShaderDataType dataType)
{
	writeShaderData(transform.matrix(), buffer, dataType);
}
