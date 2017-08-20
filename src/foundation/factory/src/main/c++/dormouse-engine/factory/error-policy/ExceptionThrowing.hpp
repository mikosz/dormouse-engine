#ifndef DORMOUSEENGINE_FACTORY_ERROR_POLICY_EXCEPTIONTHROWING_HPP_
#define DORMOUSEENGINE_FACTORY_ERROR_POLICY_EXCEPTIONTHROWING_HPP_

#include <sstream>

#include <boost/call_traits.hpp>

#include "dormouse-engine/exceptions/RuntimeError.hpp"

namespace dormouse_engine::factory::error_policy {

template <class IdentifierType>
class CreatorAlreadyRegistered : public exceptions::RuntimeError {
public:

	typedef IdentifierType Identifier;

	typedef typename boost::call_traits<Identifier>::param_type IdentifierParam;

	CreatorAlreadyRegistered(const IdentifierParam id) :
		exceptions::RuntimeError(createMessage(id)),
		id_(id)
	{
	}

	~CreatorAlreadyRegistered() throw() {
	}

	const Identifier& id() const {
		return id_;
	}

private:

	Identifier id_;

	static std::string createMessage(const IdentifierParam id) {
		std::ostringstream err;
		err << "Creator for \"" << id << "\" already registered";
		return err.str();
	}

};

template <class IdentifierType>
class NoSuchType : public exceptions::RuntimeError {
public:

	typedef IdentifierType Identifier;

	typedef typename boost::call_traits<Identifier>::param_type IdentifierParam;

	NoSuchType(const IdentifierParam id) :
		exceptions::RuntimeError(createMessage(id)),
		id_(id)
	{
	}

	~NoSuchType() throw() {
	}

	const Identifier& id() const {
		return id_;
	}

private:

	Identifier id_;

	static std::string createMessage(const IdentifierParam id) {
		std::ostringstream err;
		err << "Unknown creator id: \"" << id << "\"";
		return err.str();
	}

};

template <class IdentifierType>
class ExceptionThrowing {
public:

	typedef IdentifierType Identifier;

	typedef typename boost::call_traits<Identifier>::param_type IdentifierParam;

	static void alreadyRegistered(const IdentifierParam id) {
		throw CreatorAlreadyRegistered<Identifier>(id);
	}

	static void noSuchType(const IdentifierParam id) {
		throw NoSuchType<Identifier>(id);
	}

};

} // namespace dormouse_engine::factory::error_policy

#endif /* DORMOUSEENGINE_FACTORY_ERROR_POLICY_EXCEPTIONTHROWING_HPP_ */
