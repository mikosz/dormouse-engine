#ifndef DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_
#define DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_

#include <vector>
#include <cstdint>

namespace dormouse_engine::essentials {

using Byte = std::uint8_t;
using ByteVector = std::vector<Byte>;

class BufferView {
public:

	BufferView(size_t size, Byte* bytes) :
		size_(size),
		bytes_(bytes)
	{
	}

	size_t size() const {
		return size_;
	}

	const Byte* bytes() const {
		return bytes_;
	}

private:

	size_t size_;

	const Byte* bytes_;

};

} // namespace dormouse_engine::essentials

#endif /* DORMOUSEENGINE_ESSENTIALS_TYPES_HPP_ */
