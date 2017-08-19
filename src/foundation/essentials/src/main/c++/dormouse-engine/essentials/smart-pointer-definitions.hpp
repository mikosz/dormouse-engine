#ifndef DORMOUSEENGINE_ESSENTIALS_SMARTPOINTERDEFINITIONS_HPP_
#define DORMOUSEENGINE_ESSENTIALS_SMARTPOINTERDEFINITIONS_HPP_

#include <memory>

#define DE_SMART_POINTER_DEFINITONS(CLASS_NAME) \
	typedef std::unique_ptr<CLASS_NAME> CLASS_NAME##UniquePtr; \
	typedef std::unique_ptr<const CLASS_NAME> Const##CLASS_NAME##UniquePtr; \
	typedef std::unique_ptr<volatile CLASS_NAME> Volatile##CLASS_NAME##UniquePtr; \
	typedef std::unique_ptr<const volatile CLASS_NAME> ConstVolatile##CLASS_NAME##UniquePtr; \
	typedef std::shared_ptr<CLASS_NAME> CLASS_NAME##SharedPtr; \
	typedef std::shared_ptr<const CLASS_NAME> Const##CLASS_NAME##SharedPtr; \
	typedef std::shared_ptr<volatile CLASS_NAME> Volatile##CLASS_NAME##SharedPtr; \
	typedef std::shared_ptr<const volatile CLASS_NAME> ConstVolatile##CLASS_NAME##SharedPtr;

#endif /* DORMOUSEENGINE_ESSENTIALS_SMARTPOINTERDEFINITIONS_HPP_ */
