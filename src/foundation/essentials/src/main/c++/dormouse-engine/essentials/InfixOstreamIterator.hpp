/**
 * Copied from http://stackoverflow.com/a/3497021/1794022
 */
#ifndef _DORMOUSEENGINE_UTILS_INFIX_OSTREAM_ITERATOR_HPP_
#define _DORMOUSEENGINE_UTILS_INFIX_OSTREAM_ITERATOR_HPP_

#include <string>
#include <ostream> 

namespace dormouse_engine::essentials {

template <
	class T,
	class CharType = char,
	class TraitsType = std::char_traits<CharType>
	>
class InfixOstreamIterator
{
public:

	using iterator_category = std::output_iterator_tag;

	using value_type = void; // TODO: ?

	using difference_type = void;

	using pointer = void;

	using reference = void;

	using Ostream = std::basic_ostream<CharType, TraitsType>;

	InfixOstreamIterator(Ostream& os, std::string delimiter = std::string()) :
		os_(&os),
		delimiter_(std::move(delimiter)),
		atFirstElem_(true)
	{
	}

	InfixOstreamIterator& operator=(const T& item)
	{
		// Here's the only real change from ostream_iterator: 
		// Normally, the '*os << item;' would come before the 'if'. 
		if (!atFirstElem_ && !delimiter_.empty()) {
			*os_ << delimiter_;
		}

		*os_ << item;
		atFirstElem_ = false;
		return *this;
	}

	InfixOstreamIterator& operator*() {
		return *this;
	}

	InfixOstreamIterator& operator++() {
		return *this;
	}

	InfixOstreamIterator& operator++(int) {
		return *this;
	}

private:

	Ostream* os_;

	std::string delimiter_;

	bool atFirstElem_;

};

} // namespace dormouse_engine::essentials

namespace dormouse_engine {

using essentials::InfixOstreamIterator;

} // namespace dormouse_engine

#endif /* _DORMOUSEENGINE_UTILS_INFIX_OSTREAM_ITERATOR_HPP_ */
