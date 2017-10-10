#ifndef DORMOUSEENGINE_ESSENTIALS_POLICY_NONE_HPP_
#define DORMOUSEENGINE_ESSENTIALS_POLICY_NONE_HPP_

#include <memory>

namespace dormouse_engine::essentials::policy::creation {

template <class InstanceType, class... Arguments>
class None {
public:

	using Instance = std::unique_ptr<InstanceType>;

	Instance create(Arguments&&...) {
		return Instance();
	}

};

} // namespace dormouse_engine::essentials::policy::creation

#endif /* DORMOUSEENGINE_ESSENTIALS_POLICY_NONE_HPP_ */
