#ifndef DORMOUSEENGINE_UTILS_RANGE_HPP_
#define DORMOUSEENGINE_UTILS_RANGE_HPP_

#include <type_traits>

#include <boost/operators.hpp>

namespace dormouse_engine::essentials {

template <class T, size_t EPSILON_RECIPROCAL = 10000u> // TODO: epsilon
class Range {
public:

	class Iterator : public boost::bidirectional_iterator_helper<Iterator, const T> {
	public:

		Iterator() = default;

		explicit Iterator(T current, T step) :
			current_(current),
			step_(step)
		{
		}

		const T& operator*() const {
			return current_;
		}

		Iterator& operator++() {
			current_ += step_;
			return *this;
		}

		Iterator& operator--() {
			current_ -= step_;
			return *this;
		}

		bool operator==(const Iterator& other) const {
			return Range::equal(current_, other.current_);
		}

	private:

		T current_;

		T step_;

		friend class ReverseIterator;

	};

	class ReverseIterator : public boost::bidirectional_iterator_helper<ReverseIterator, const T> {
	public:

		ReverseIterator() = default;

		ReverseIterator(Iterator forward) :
			current_(*forward - forward.step_),
			step_(forward.step_)
		{
		}

		const T& operator*() const {
			return current_;
		}

		ReverseIterator& operator++() {
			current_ -= step_;
			return *this;
		}

		ReverseIterator& operator--() {
			current_ += step_;
			return *this;
		}

		bool operator==(const ReverseIterator& other) const {
			return Range::equal(current_, other.current_);
		}

	private:

		T current_;

		T step_;

	};

	using iterator = Iterator;
	using const_iterator = Iterator;

	Range(T begin, T end, T step = T(1)) :
		begin_(begin),
		end_(end),
		step_(step)
	{
	}

	Iterator begin() const {
		return Iterator(begin_, step_);
	}

	Iterator end() const {
		return Iterator(end_, step_);
	}

	ReverseIterator rbegin() const {
		return ReverseIterator(Iterator(end_, step_));
	}

	ReverseIterator rend() const {
		return ReverseIterator(Iterator(begin_, step_));
	}

private:

	T begin_;

	T end_;

	T step_;

	template <class P>
	static bool equal(P lhs, P rhs, std::enable_if_t<std::is_floating_point<P>::value>* = nullptr) {
		return std::abs(lhs - rhs) < (P(1) / EPSILON_RECIPROCAL);
	}

	template <class P>
	static bool equal(P lhs, P rhs, std::enable_if_t<!std::is_floating_point<P>::value>* = nullptr) {
		return lhs == rhs;
	}

};

using IndexRange = Range<size_t>;

template <class T>
Range<T> range(T begin, T end, T step = T(1)) {
	return Range<T>(std::move(begin), std::move(end), std::move(step));
}

} // namespace dormouse_engine::essentials

namespace dormouse_engine {

using essentials::Range;
using essentials::range;
using essentials::IndexRange;

} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_UTILS_INDEXRANGE_HPP_ */
