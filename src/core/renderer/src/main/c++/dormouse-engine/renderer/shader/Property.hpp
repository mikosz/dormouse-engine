#ifndef _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_
#define _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_

#include <memory>
#include <type_traits>
#include <typeinfo>

#include "dormouse-engine/exceptions/RuntimeError.hpp"
#include "dormouse-engine/essentials/StringId.hpp"
#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/essentials/memory.hpp"
#include "dormouse-engine/graphics/ShaderType.hpp"
#include "../control/controlfwd.hpp"
#include "../command/commandfwd.hpp"

namespace dormouse_engine::renderer::shader {

class PropertyNotBound final : public exceptions::RuntimeError {
public:

	PropertyNotBound(essentials::StringId id) :
		exceptions::RuntimeError("Required property \"" + id.string() + "\" is not bound")
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

class Property final {
public:

	Property() {
		new(&object_) Default;
	}

	template <class T>
	Property(T model) {
		static_assert(sizeof(Model<T>) <= STORAGE_SIZE);
		static_assert(alignof(Model<T>) <= STORAGE_ALIGNMENT);
		static_assert(std::is_trivially_copyable_v<T>);
		new(&object_) Model<T>(std::move(model));
	}

	~Property() {
		reinterpret_cast<Concept*>(&object_)->~Concept();
	}

	bool has(essentials::StringId id) const {
		assert(&object_ != nullptr);
		return reinterpret_cast<const Concept*>(&object_)->has(std::move(id));
	}

	Property get(essentials::StringId id) const {
		assert(&object_ != nullptr);
		return reinterpret_cast<const Concept*>(&object_)->get(std::move(id));
	}

	void bindResource(command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot) const {
		assert(&object_ != nullptr);
		reinterpret_cast<const Concept*>(&object_)->bindResource(cmd, stage, slot);
	}

private:

	class Concept {
	public:

		virtual ~Concept() = default;

		virtual bool has(essentials::StringId id) const = 0;

		virtual Property get(essentials::StringId id) const = 0;

		virtual void bindResource(command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot) const = 0;

	};

	class Default : public Concept {

		bool has(essentials::StringId /*id*/) const override {
			return false;
		}

		Property get(essentials::StringId id) const override {
			throw PropertyNotBound(std::move(id));
		}

		void bindResource(
			command::DrawCommand& /*cmd*/, graphics::ShaderType /*stage*/, size_t /*slot*/) const override
		{
			throw NotAResourceProperty();
		}

	};

	template <class T>
	class Model : public Concept {
	public:

		Model(T model) :
			model_(std::move(model))
		{
		}

		bool has(essentials::StringId id) const override {
			return hasShaderProperty(model_, std::move(id));
		}

		Property get(essentials::StringId id) const override {
			return getShaderProperty(model_, std::move(id));
		}

		void bindResource(command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot) const override {
			bindShaderResource(model_, cmd, stage, slot);
		}

	private:

		T model_;

	};

	static constexpr auto STORAGE_SIZE = 3 * sizeof(void*);

	static constexpr auto STORAGE_ALIGNMENT = alignof(void*);

	std::aligned_storage_t<STORAGE_SIZE, STORAGE_ALIGNMENT> object_;

};

template <class T>
inline bool hasShaderProperty(const T& /*model*/, essentials::StringId /*id*/) {
	return false;
}

template <class T>
inline Property getShaderProperty(const T& /*model*/, essentials::StringId id) {
	throw PropertyNotBound(std::move(id));
}

template <class T>
inline void bindShaderResource(
	const T& /*model*/, command::DrawCommand& /*cmd*/, graphics::ShaderType /*stage*/, size_t /*slot*/)
{
	throw NotAResourceProperty();
}

void bindShaderResource(
	control::ResourceView resourceView, command::DrawCommand& cmd, graphics::ShaderType stage, size_t slot);

} // namespace dormouse_engine::renderer::shader

#endif /* _DORMOUSEENGINE_RENDERER_SHADER_PROPERTY_HPP_ */
