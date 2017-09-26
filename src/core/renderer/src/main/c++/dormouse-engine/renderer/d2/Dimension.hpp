#ifndef _DORMOUSEENGINE_RENDERER_D2_DIMENSION_HPP_
#define _DORMOUSEENGINE_RENDERER_D2_DIMENSION_HPP_

#include <functional>

#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/essentials/PolymorphicStorage.hpp"

namespace dormouse_engine::renderer::d2 {

class Dimension;

class Ndc final {
public:

	Ndc(float value) noexcept :
		value_(value)
	{
	}

	float ndc() const noexcept {
		return value_;
	}

private:

	float value_;

};

class WindowRelative final {
public:

	WindowRelative(float value) noexcept :
		value_(value)
	{
	}

	float ndc() const noexcept {
		return (value_ * 2.0f) - 1.0f;
	}

private:

	float value_;

};

class Pixel final {
public:

	using SizeFunc = std::function<size_t()>;

	Pixel(size_t value, SizeFunc sizeFunc) noexcept :
	value_(value),
		sizeFunc_(std::move(sizeFunc))
	{
	}

	float ndc() const noexcept {
		return 2.0f * static_cast<float>(value_) / static_cast<float>(sizeFunc_()) - 1.0f;
	}

private:

	size_t value_;

	SizeFunc sizeFunc_;

};

class RatioKeeping final {
public:

	RatioKeeping(essentials::observer_ptr<const Dimension> other, float ratio) :
		other_(std::move(other)),
		ratio_(ratio)
	{
	}

	float ndc() const noexcept;

private:

	essentials::observer_ptr<const Dimension> other_;

	float ratio_;

};

class Dimension final {
public:

	template <class T>
	Dimension(T model) :
		storage_(std::move(model))
	{
	}

	float ndc() const noexcept {
		return storage_->ndc();
	}

private:

	class Concept : public essentials::ConceptBase {
	public:

		virtual float ndc() const noexcept = 0;

	};

	template <class T>
	class Model : public essentials::ModelBase<Concept, Model<T>, T> {
	public:

		Model(T model) :
			essentials::ModelBase<Concept, Model<T>, T>(std::move(model))
		{
		}

		float ndc() const noexcept override {
			return model_.ndc();
		}

	};

	static constexpr auto STORAGE_SIZE = sizeof(Model<Pixel>);
	static constexpr auto STORAGE_ALIGNMENT = alignof(Model<Pixel>);

	essentials::PolymorphicStorage<Concept, Model, STORAGE_SIZE, STORAGE_ALIGNMENT> storage_;

};

inline float RatioKeeping::ndc() const noexcept {
	return ratio_ * other_->ndc();
}

} // namespace dormouse_engine::renderer::d2

#endif /* _DORMOUSEENGINE_RENDERER_D2_DIMENSION_HPP_ */
