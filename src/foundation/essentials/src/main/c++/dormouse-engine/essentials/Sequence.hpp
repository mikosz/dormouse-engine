#ifndef DORMOUSEENGINE_UTILS_SEQUENCE_HPP_
#define DORMOUSEENGINE_UTILS_SEQUENCE_HPP_

#include <boost/call_traits.hpp>

#include <utility>

namespace dormouse_engine::essentials {

template <class T>
class Sequence {
public:

	Sequence(const typename boost::call_traits<T>::param_type first,
			const typename boost::call_traits<T>::param_type second) :
			pair_(first, second) {
	}

	Sequence(const std::pair<T, T> pair) :
		pair_(pair) {
	}

	operator const std::pair<T, T>&() const {
		return pair();
	}

	const std::pair<T, T>& pair() const {
		return pair_;
	}

	bool atEnd() const {
		return pair_.first == pair_.second;
	}

	void next() {
		++pair_.first;
	}

	T& current() {
		return pair_.first;
	}

private:

	std::pair<T, T> pair_;

};

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_UTILS_SEQUENCE_HPP_ */
