#ifndef DORMOUSEENGINE_EXCEPTIONS_GCC_BACKTRACE_HPP_
#define DORMOUSEENGINE_EXCEPTIONS_GCC_BACKTRACE_HPP_

#include <string>
#include <vector>
#include <iosfwd>

namespace dormouse_engine {
namespace exceptions {
namespace gcc {

class Backtrace {
public:

	class Frame {
	public:

		Frame(const std::string& mangledName);

		const std::string& name() const {
			return name_;
		}

	private:

		std::string name_;

	};

	typedef std::vector<Frame> Stack;

	Backtrace();

	const Stack& stack() const {
		return stack_;
	}

	std::ostream& print(std::ostream& os) const;

private:

	Stack stack_;

};

} // namespace gcc
} // namespace exceptions
} // namespace dormouse_engine

#endif /* DORMOUSEENGINE_EXCEPTIONS_GCC_BACKTRACE_HPP_ */
