#ifndef DORMOUSEENGINE_UTILS_INITIALISABLE_HPP_
#define DORMOUSEENGINE_UTILS_INITIALISABLE_HPP_

#include <memory>

#include "Initialiser.hpp"

#include "dormouse-engine/essentials/smart-pointer-definitions.hpp"

namespace dormouse_engine::essentials {

template <class ValueType, class... InitialiseArguments>
class Initialisable {
public:

	typedef ValueType Value;

	typedef Initialiser<Value, InitialiseArguments...> Initialiser;

protected:

	Initialisable() {
	}

	virtual ~Initialisable() {
	}

	void initialise(InitialiseArguments... arguments) {
		doInitialise(arguments...);
	}

	virtual void doInitialise(InitialiseArguments... arguments) = 0;

	friend class essentials::Initialiser<Value, InitialiseArguments...>;

};

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_UTILS_INITIALISABLE_HPP_ */
