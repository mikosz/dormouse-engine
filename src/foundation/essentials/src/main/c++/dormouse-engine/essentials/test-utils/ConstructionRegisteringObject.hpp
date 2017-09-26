#ifndef DORMOUSEENGINE_ESSENTIALS_TEST_UTILS_CONSTRUCTIONREGISTERINGOBJECT_HPP_
#define DORMOUSEENGINE_ESSENTIALS_TEST_UTILS_CONSTRUCTIONREGISTERINGOBJECT_HPP_

#include <map>

namespace dormouse_engine::essentials::test_utils {

struct ConstructionRegistry {

	struct Status {
		bool defaultConstructed = false;
		bool copyConstructed = false;
		bool moveConstructed = false;
		bool destructed = false;
		bool copyAssignedTo = false;
		bool moveAssignedTo = false;
		bool movedFrom = false;
	};

	std::map<const void*, Status> objects;

};

class ConstructionRegisteringObject {
public:

	ConstructionRegisteringObject(ConstructionRegistry& registry) :
		registry_(registry)
	{
		registry_.objects[this].defaultConstructed = true;
	}

	ConstructionRegisteringObject(const ConstructionRegisteringObject& other) :
		registry_(other.registry_)
	{
		registry_.objects[this].copyConstructed = true;
	}

	ConstructionRegisteringObject(ConstructionRegisteringObject&& other) :
		registry_(other.registry_)
	{
		registry_.objects[this].moveConstructed = true;
		registry_.objects[&other].movedFrom = true;
	}

	~ConstructionRegisteringObject() {
		registry_.objects[this].destructed = true;
	}

	ConstructionRegisteringObject& operator=([[maybe_unused]] const ConstructionRegisteringObject& other) {
		registry_.objects[this].copyAssignedTo = true;
	}

	ConstructionRegisteringObject& operator=(ConstructionRegisteringObject&& other) {
		registry_.objects[this].moveAssignedTo = true;
		registry_.objects[&other].movedFrom = true;
	}

private:

	ConstructionRegistry& registry_;

};

} // namespace dormouse_engine::essentials::test_utils

#endif /* DORMOUSEENGINE_ESSENTIALS_TEST_UTILS_CONSTRUCTIONREGISTERINGOBJECT_HPP_ */
