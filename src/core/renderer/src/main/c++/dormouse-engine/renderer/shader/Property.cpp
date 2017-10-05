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
