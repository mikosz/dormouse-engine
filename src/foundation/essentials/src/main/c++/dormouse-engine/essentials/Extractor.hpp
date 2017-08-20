#ifndef DORMOUSEENGINE_UTILS_EXTRACTOR_HPP_
#define DORMOUSEENGINE_UTILS_EXTRACTOR_HPP_

#include <functional>

namespace dormouse_engine::essentials {

template <class Owner, class Member>
class Extractor : public std::unary_function<const Owner&, const Member&> {
public:

	Extractor(Member Owner::*member) :
		member_(member) {
	}

	const Member& operator()(const Owner& t) const {
		return t.*member_;
	}

private:

	Member Owner::*member_;

};

template <class Owner, class Member>
Extractor<Owner, Member> makeExtractor(Member Owner::*member) {
	return Extractor<Owner, Member>(member);
}

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_UTILS_EXTRACTOR_HPP_ */
