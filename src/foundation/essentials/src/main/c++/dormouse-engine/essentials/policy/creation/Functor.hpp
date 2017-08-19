#ifndef DORMOUSEENGINE_POLICY_CREATION_HPP_
#define DORMOUSEENGINE_POLICY_CREATION_HPP_

#include <memory>
#include <type_traits>

namespace dormouse_engine {
namespace policy {
namespace creation {

template <class FunctionType, class... Arguments>
class Functor {
public:

	using Instance = std::result_of_t<FunctionType(Arguments...)>;

	template <class CompatibleFunctionType>
	Functor(CompatibleFunctionType creator) :
		creator_(creator) {
	}

	Instance create(Arguments&&... arguments) {
		return creator_(std::forward<Arguments>(arguments)...);
	}

private:

	FunctionType creator_;

};

template <class... Arguments, class FunctionType>
Functor<FunctionType, Arguments...> makeFunctor(FunctionType creator) { // TODO: unnecessary?
	return Functor<FunctionType, Arguments...>(std::move(creator));
}

} // namespace creation
} // namespace policy
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_POLICY_CREATION_HPP_ */
