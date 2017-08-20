#ifndef DORMOUSEENGINE_LOGGER_CATEGORY_HPP_
#define DORMOUSEENGINE_LOGGER_CATEGORY_HPP_

#include <string>

namespace dormouse_engine::logger {

typedef std::string Category;

/*
 * The following structures and loggerCategory function are built so that it's possible
 * to use the DE_LOGGER_CATEGORY macros or not. FakeParam is needed so that argument-dependent
 * lookup does not make categories defined by the macro ambiguous.
 */
struct FakeParam {
};

struct ConstructibleFromFakeParam {

	ConstructibleFromFakeParam(FakeParam) {
	}

};

inline const Category& loggerCategory(ConstructibleFromFakeParam = FakeParam()) {
	static Category category = "";
	return category;
}

} // namespace dormouse_engine::logger

#endif /* DORMOUSEENGINE_LOGGER_CATEGORY_HPP_ */
