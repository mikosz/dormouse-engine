#ifndef DORMOUSEENGINE_UTILS_RAII_HELPER_HPP_
#define DORMOUSEENGINE_UTILS_RAII_HELPER_HPP_

#include <functional>

namespace dormouse_engine::essentials {

class RaiiHelper {
public:

	typedef std::function<void()> Callback;

	RaiiHelper(Callback f) :
		f_(f) {
	}

	~RaiiHelper() {
		if (f_) {
			f_();
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
