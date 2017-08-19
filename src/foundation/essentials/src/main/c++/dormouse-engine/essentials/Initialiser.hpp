#ifndef DORMOUSEENGINE_UTILS_INITIALISER_HPP_
#define DORMOUSEENGINE_UTILS_INITIALISER_HPP_

#include <memory>

#include <boost/noncopyable.hpp>

#include "dormouse-engine/exceptions/LogicError.hpp"

namespace dormouse_engine {
namespace essentials {

class ObjectAlreadyInitialised : public exceptions::LogicError {
};

template <class InitialisableType, class... Arguments>
class Initialiser : boost::noncopyable {
public:

	typedef InitialisableType Initialisable;

	typedef std::unique_ptr<Initialisable> Ptr;

	template <class ConcreteType>
	static Initialiser createInitialisable() {
		return Ptr(new ConcreteType);
	}

	Initialiser(Initialiser&& other) :
		initialisable_(std::move(other.initialisable_))
	{
	}

	Ptr initialise(Arguments... arguments) {
		if (!initialisable_) {
			throw ObjectAlreadyInitialised();
		}

		initialisable_->initialise(arguments...);
		return Ptr(initialisable_.release());
	}

	bool empty() const {
		return initialisable_;
	}

private:

	Ptr initialisable_;

	Initialiser(Ptr&& initialisable) :
		initialisable_(std::move(initialisable))
	{
	}

};

}  // namespace essentials
}  // namespace dormouse_engine

#endif /* DORMOUSEENGINE_UTILS_INITIALISER_HPP_ */
