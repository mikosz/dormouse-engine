#ifndef DORMOUSEENGINE_UTILS_RAII_HELPER_HPP_
#define DORMOUSEENGINE_UTILS_RAII_HELPER_HPP_

#include <functional>

namespace dormouse_engine::essentials {

class RaiiHelper {
public:

	typedef std::function<void()> Callback;

	RaiiHelper(Callback f) noexcept :
		f_(f) {
	}

	~RaiiHelper() noexcept {
		if (f_) {
			f_();
		}
	}

	RaiiHelper(RaiiHelper&& other) noexcept :
		f_(other.f_)
	{
		other.reset();
	}

	RaiiHelper& operator=(RaiiHelper&& other) noexcept {
		if (this != &other) {
			f_ = other.f_;
			other.reset();
		}
	}

	void reset() {
		f_ = Callback();
	}

private:

	Callback f_;

};

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_UTILS_RAII_HELPER_HPP_ */
