#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_

#include <memory>
#include <type_traits>
#include <typeinfo>
#include <cassert>

#include <boost/lexical_cast.hpp>

#include "dormouse-engine/exceptions/RuntimeError.hpp"
#include "dormouse-engine/essentials/StringId.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/essentials/PolymorphicStorage.hpp"
#include "dormouse-engine/reflection/Object.hpp"
#include "dormouse-engine/graphics/ShaderType.hpp"
#include "dormouse-engine/graphics/ShaderDataType.hpp"
#include "dormouse-engine/math/Matrix.hpp"
#include "dormouse-engine/math/Transform.hpp"
#include "../control/controlfwd.hpp"
#include "../command/commandfwd.hpp"
#include "PropertyId.hpp"

namespace dormouse_engine::renderer::shader {

class PropertyNotBound final : public exceptions::RuntimeError {
public:

	PropertyNotBound(const PropertyId& fullId) :
		exceptions::RuntimeError(
			"Required property \"" + boost::lexical_cast<std::string>(fullId) + "\" is not bound")
	{
	}

};

class NotAResourceProperty final : public exceptions::RuntimeError {
public:

	NotAResourceProperty() :
		exceptions::RuntimeError("Accessed property is not a resource property")
	{
	}

};

class PropertyNotWriteable final : public exceptions::RuntimeError {
public:

	PropertyNotWriteable() :
		exceptions::RuntimeError("Accessed property is not writeable")
	{
	}

};

class IncompatibleDataType final : public exceptions::RuntimeError {
public:

	IncompatibleDataType(const std::string& message) :
		exceptions::RuntimeError(message)
	{
	}

};

class Property final {
public:

	Property() :
		storage_(Default())
	{
	}

	template <class T>
	Property(T model) :
		storage_(std::move(model))
	{
	}

	bool has(essentials::StringId id, size_t arrayIdx = 0u) const {
		return storage_->has(std::move(id), arrayIdx);
	}

	Property get(essentials::StringId id, size_t arrayIdx = 0u) const {
		return storage_->get(std::move(id), arrayIdx);
	}

	void bindResource(command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot) const {
		storage_->bindResource(cmd, stage, slot);
	}

	void write(essentials::BufferView buffer, graphics::ShaderDataType dataType) const {
		storage_->write(buffer, dataType);
	}

private:

	class Concept : public essentials::ConceptBase {
	public:

		virtual ~Concept() = default;

		virtual bool has(essentials::StringId id, size_t arrayIdx) const = 0;

		virtual Property get(essentials::StringId id, size_t arrayIdx) const = 0;

		virtual void bindResource(command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot) const = 0;

		virtual void write(essentials::BufferView buffer, graphics::ShaderDataType dataType) const = 0;

	};

	template <class T>
	class Model : public essentials::ModelBase<Concept, Model<T>, T> {
	public:

		Model(T model) :
			essentials::ModelBase<Concept, Model<T>, T>(std::move(model))
		{
		}

		bool has(essentials::StringId id, size_t arrayIdx) const override {
			return hasShaderProperty(model_, std::move(id), arrayIdx);
		}

		Property get(essentials::StringId id, size_t arrayIdx) const override {
			return getShaderProperty(model_, std::move(id), arrayIdx);
		}

		void bindResource(command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot) const override {
			bindShaderResource(model_, cmd, stage, slot);
		}

		void write(essentials::BufferView buffer, graphics::ShaderDataType dataType) const {
			writeShaderData(model_, buffer, dataType);
		}

	};

	class Default : public Model<nullptr_t> {
	public:

		Default() :
			Model<nullptr_t>(nullptr)
		{
		}

		bool has([[maybe_unused]] essentials::StringId id, [[maybe_unused]] size_t arrayIdx) const override {
			return false;
		}

		Property get([[maybe_unused]] essentials::StringId id, [[maybe_unused]] size_t arrayIdx) const override {
			assert(!"Property not bound");
			return Property();
		}

		void bindResource(
			[[maybe_unused]] command::DrawCommand& cmd,
			[[maybe_unused]] graphics::ShaderType stage,
			[[maybe_unused]] size_t slot
			) const override
		{
			throw NotAResourceProperty();
		}

		void write(
			[[maybe_unused]] essentials::BufferView buffer,
			[[maybe_unused]] graphics::ShaderDataType dataType
			) const override
		{
			throw PropertyNotWriteable();
		}

	};

	static constexpr auto STORAGE_SIZE = sizeof(void*) + 16 * sizeof(float); // matrix
	static constexpr auto STORAGE_ALIGNMENT = alignof(void*);

	essentials::PolymorphicStorage<Concept, Model, STORAGE_SIZE, STORAGE_ALIGNMENT> storage_;

};

template <class T>
inline bool hasShaderProperty(
	[[maybe_unused]] const T& model,
	[[maybe_unused]] essentials::StringId id,
	[[maybe_unused]] size_t arrayIdx
	)
{
	if constexpr (essentials::IsAnyPointer_v<T>) {
		return hasShaderProperty(*model, std::move(id), arrayIdx);
	} else {
		return false;
	}
}

bool hasShaderProperty(
	reflection::Object reflectionObject,
	essentials::StringId id,
	size_t arrayIdx
	);

template <class T>
inline Property getShaderProperty(
	[[maybe_unused]] const T& model,
	[[maybe_unused]] essentials::StringId id,
	[[maybe_unused]] size_t arrayIdx
	)
{
	if constexpr (essentials::IsAnyPointer_v<T>) {
		return getShaderProperty(*model, std::move(id), arrayIdx);
	} else {
		assert(!"Property not bound");
		return Property();
	}
}

Property getShaderProperty(
	reflection::Object reflectionObject,
	essentials::StringId id,
	size_t arrayIdx
	);

template <class T>
inline void bindShaderResource(
	[[maybe_unused]] const T& model,
	[[maybe_unused]] command::DrawCommand& cmd,
	[[maybe_unused]] graphics::ShaderType stage,
	[[maybe_unused]] size_t slot
	)
{
	if constexpr (essentials::IsAnyPointer_v<T>) {
		bindShaderResource(*model, cmd, stage, slot);
	} else {
		throw NotAResourceProperty();
	}
}

template <class T>
inline void writeShaderData(
	[[maybe_unused]] const T& model,
	[[maybe_unused]] essentials::BufferView buffer,
	[[maybe_unused]] graphics::ShaderDataType dataType
	)
{
	if constexpr (essentials::IsAnyPointer_v<T>) {
		writeShaderData(*model, buffer, dataType);
	} else {
		throw PropertyNotWriteable();
	}
}

void bindShaderResource(
	control::ResourceView resourceView, command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot);
void bindShaderResource(
	control::Sampler sampler, command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot);

void writeShaderData(
	const math::Matrix4x4& matrix, essentials::BufferView buffer, graphics::ShaderDataType dataType);
void writeShaderData(
	const math::Transform& transform, essentials::BufferView buffer, graphics::ShaderDataType dataType);

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_ */
