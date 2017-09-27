#ifndef _DORMOUSEENGINE_RENDERER_D2_DIMENSION_HPP_
#define _DORMOUSEENGINE_RENDERER_D2_DIMENSION_HPP_

#include <functional>

#include "dormouse-engine/essentials/observer_ptr.hpp"
#include "dormouse-engine/essentials/PolymorphicStorage.hpp"

namespace dormouse_engine::renderer::d2 {

class Dimension;

class Ndc final {
public:

	explicit Ndc(float value) noexcept :
		value_(value)
	{
	}

	float ndcPoint() const noexcept {
		return value_;
	}

	float ndcVector() const noexcept {
		return value_;
	}

private:

	float value_;

};

class WindowRelative final {
public:

	explicit WindowRelative(float value) noexcept :
		value_(value)
	{
	}

	float ndcPoint() const noexcept {
		return ndcVector() - 1.0f;
	}

	float ndcVector() const noexcept {
		return value_ * 2.0f;
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

	float ndcPoint() const noexcept {
		return ndcVector() - 1.0f;
	}

	float ndcVector() const noexcept {
		return 2.0f * static_cast<float>(value_) / static_cast<float>(sizeFunc_());
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

	float ndcPoint() const noexcept;
	
	float ndcVector() const noexcept;

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

	float ndcPoint() const noexcept {
		return storage_->ndcPoint();
	}

	float ndcVector() const noexcept {
		return storage_->ndcVector();
	}

private:

	class Concept : public essentials::ConceptBase {
	public:

		virtual float ndcPoint() const noexcept = 0;

		virtual float ndcVector() const noexcept = 0;

	};

	template <class T>
	class Model : public essentials::ModelBase<Concept, Model<T>, T> {
	public:

		Model(T model) :
			essentials::ModelBase<Concept, Model<T>, T>(std::move(model))
		{
		}

		float ndcPoint() const noexcept override {
			return model_.ndcPoint();
		}

		float ndcVector() const noexcept override {
			return model_.ndcVector();
		}

	};

	static constexpr auto STORAGE_SIZE = sizeof(Model<Pixel>);
	static constexpr auto STORAGE_ALIGNMENT = alignof(Model<Pixel>);

	essentials::PolymorphicStorage<Concept, Model, STORAGE_SIZE, STORAGE_ALIGNMENT> storage_;

};

inline float RatioKeeping::ndcVector() const noexcept {
	return ratio_ * other_->ndcVector();
}

inline float RatioKeeping::ndcPoint() const noexcept {
	return ratio_ * other_->ndcPoint();
}

} // namespace dormouse_engine::renderer::d2

#endif /* _DORMOUSEENGINE_RENDERER_D2_DIMENSION_HPP_ */
