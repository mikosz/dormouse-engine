#ifndef _DORMOUSEENGINE_TESTER_APP_HPP_
#define _DORMOUSEENGINE_TESTER_APP_HPP_

#include "dormouse-engine/engine/app/App.hpp"

namespace dormouse_engine::tester {

class App final {
public:

	App();

	void update();

private:

	engine::app::App engineApp_;

};

} // namespace dormouse_engine::tester

#endif /* _DORMOUSEENGINE_TESTER_APP_HPP_ */
