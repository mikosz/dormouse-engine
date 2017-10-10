#ifndef DORMOUSEENGINE_ESSENTIALS_FUNCTIONAL_HPP_
#define DORMOUSEENGINE_ESSENTIALS_FUNCTIONAL_HPP_

namespace dormouse_engine::essentials {

struct Identity {

	template<typename T>
	constexpr auto operator()(T&& v) const noexcept {
		return std::forward<T>(v);
	}

};

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_ESSENTIALS_FUNCTIONAL_HPP_ */
