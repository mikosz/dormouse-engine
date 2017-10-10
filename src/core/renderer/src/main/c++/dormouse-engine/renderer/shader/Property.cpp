#include "Property.hpp"

#pragma warning(push, 3)
#	include <ponder/valuevisitor.hpp>
#	include <ponder/classvisitor.hpp>
#	include <ponder/uses/detail/runtime.hpp> // TODO: wtf?
#	include <ponder/uses/runtime.hpp>
#pragma warning(pop)

#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/exceptions/LogicError.hpp"
#include "dormouse-engine/exceptions/RuntimeError.hpp"
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

class FunctionFindingVisitor : public ponder::ClassVisitor {
public:

	FunctionFindingVisitor(reflection::FunctionTag findTag) :
		findTag_(findTag)
	{
	}

	void visit(const ponder::Function& function) override {
		if (function.hasTag(findTag_)) {
			if (function_) {
				throw exceptions::LogicError(
					"Class has multiple functions tagged with " +
					toString(findTag_) +
					": " +
					function_->name() +
					", " +
					function.name()
					);
			}
		}

		function_ = essentials::make_observer(&function);
	}

	essentials::observer_ptr<const ponder::Function> function() const {
		return function_;
	}

private:

	reflection::FunctionTag findTag_;

	essentials::observer_ptr<const ponder::Function> function_;

};

template <class... ArgTypes>
void findAndCallTagged(
	reflection::Object reflectionObject,
	reflection::ClassTag requiredClassTag,
	reflection::FunctionTag requiredFunctionTag,
	ArgTypes&&... args
	)
{
	const auto& metaclass = reflectionObject.metaclass();

	if (!metaclass.hasTag(requiredClassTag)) {
		throw exceptions::RuntimeError(
			"Class " +
			metaclass.name() +
			" is not tagged as " +
			toString(requiredClassTag)
			);
	}

	auto binderVisitor = FunctionFindingVisitor(requiredFunctionTag);
	metaclass.visit(binderVisitor);

	auto functionPtr = binderVisitor.function();
	if (!functionPtr) {
		throw exceptions::LogicError(
			"Class " +
			metaclass.name() +
			" tagged as " +
			toString(requiredClassTag) +
			" has no functions tagged as " +
			toString(requiredFunctionTag)
			);
	}

	const auto functionKind = functionPtr->kind();
	if (
		functionKind == ponder::FunctionKind::MemberFunction ||
		functionKind == ponder::FunctionKind::MemberObject
		)
	{
		ponder::runtime::call(
			*functionPtr,
			reflectionObject.metaobject(),
			std::forward<ArgTypes>(args)...
		);
	} else {
		ponder::runtime::callStatic(
			*functionPtr,
			reflectionObject.metaobject(),
			std::forward<ArgTypes>(args)...
			);
	}
}

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
	reflection::Object reflectionObject,
	command::DrawCommand& cmd,
	graphics::ShaderType stage,
	size_t slot
	)
{
	findAndCallTagged(
		reflectionObject,
		reflection::ClassTag::SHADER_RESOURCE,
		reflection::FunctionTag::BIND_SHADER_RESOURCE,
		ponder::UserObject::makeRef(cmd),
		stage,
		slot
		);
}

void shader::writeShaderData(
	reflection::Object reflectionObject,
	essentials::BufferView buffer,
	graphics::ShaderDataType dataType
	)
{
	findAndCallTagged(
		reflectionObject,
		reflection::ClassTag::SHADER_DATA,
		reflection::FunctionTag::WRITE_SHADER_DATA,
		ponder::UserObject::makeRef(buffer),
		ponder::UserObject::makeRef(dataType)
		);
}
