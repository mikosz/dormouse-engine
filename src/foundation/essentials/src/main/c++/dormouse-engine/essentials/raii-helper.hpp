#ifndef DORMOUSEENGINE_UTILS_RAII_HELPER_HPP_
#define DORMOUSEENGINE_UTILS_RAII_HELPER_HPP_

#include <functional>

namespace dormouse_engine {
namespace essentials {

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

}  // namespace essentials
}  // namespace CoconutTools

#endif /* DORMOUSEENGINE_UTILS_RAII_HELPER_HPP_ */
