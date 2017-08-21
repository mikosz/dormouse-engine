#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "dormouse-engine/essentials/memory.hpp"

#include <type_traits>

namespace /* anonymous */ {

using namespace dormouse_engine::essentials;
using namespace std::string_literals;

BOOST_AUTO_TEST_SUITE(MemoryTestSuite);

BOOST_AUTO_TEST_CASE(BufferViewViewsRawData) {
	int ints[] = { 1, 2, 3 };

	auto* intsPtr = ints;
	auto view = viewBuffer(intsPtr, sizeof(ints) / sizeof(ints[0]));

	static_assert(std::is_same_v<decltype(view)::ByteType, Byte>);
	BOOST_CHECK_EQUAL(view.data(), reinterpret_cast<Byte*>(ints));
	BOOST_CHECK_EQUAL(view.size(), sizeof(ints));
}

BOOST_AUTO_TEST_CASE(BufferViewViewsConstRawData) {
	const int ints[] = { 1, 2, 3 };

	const auto* intsPtr = ints;
	auto view = viewBuffer(intsPtr, sizeof(ints) / sizeof(ints[0]));

	static_assert(std::is_same_v<decltype(view)::ByteType, const Byte>);
	BOOST_CHECK_EQUAL(view.data(), reinterpret_cast<const Byte*>(ints));
	BOOST_CHECK_EQUAL(view.size(), sizeof(ints));
}

BOOST_AUTO_TEST_CASE(BufferViewViewsCArray) {
	int ints[] = { 1, 2, 3 };

	auto view = viewBuffer(ints);

	static_assert(std::is_same_v<decltype(view)::ByteType, Byte>);
	BOOST_CHECK_EQUAL(view.data(), reinterpret_cast<Byte*>(ints));
	BOOST_CHECK_EQUAL(view.size(), sizeof(ints));
}

BOOST_AUTO_TEST_CASE(BufferViewViewsConstCArray) {
	const int ints[] = { 1, 2, 3 };

	auto view = viewBuffer(ints);

	static_assert(std::is_same_v<decltype(view)::ByteType, const Byte>);
	BOOST_CHECK_EQUAL(view.data(), reinterpret_cast<const Byte*>(ints));
	BOOST_CHECK_EQUAL(view.size(), sizeof(ints));
}

BOOST_AUTO_TEST_CASE(BufferViewViewsStdArray) {
	std::array<int, 3> ints = { 1, 2, 3 };

	auto view = viewBuffer(ints);

	static_assert(std::is_same_v<decltype(view)::ByteType, Byte>);
	BOOST_CHECK_EQUAL(view.data(), reinterpret_cast<Byte*>(&ints[0]));
	BOOST_CHECK_EQUAL(view.size(), sizeof(ints));
}

BOOST_AUTO_TEST_CASE(BufferViewViewsConstStdArray) {
	const std::array<int, 3> ints = { 1, 2, 3 };

	auto view = viewBuffer(ints);

	static_assert(std::is_same_v<decltype(view)::ByteType, const Byte>);
	BOOST_CHECK_EQUAL(view.data(), reinterpret_cast<const Byte*>(&ints[0]));
	BOOST_CHECK_EQUAL(view.size(), sizeof(ints));
}

BOOST_AUTO_TEST_CASE(BufferViewViewsStdVector) {
	auto ints = std::vector<int>{ 1, 2, 3 };

	auto view = viewBuffer(ints);

	static_assert(std::is_same_v<decltype(view)::ByteType, Byte>);
	BOOST_CHECK_EQUAL(view.data(), reinterpret_cast<Byte*>(&ints[0]));
	BOOST_CHECK_EQUAL(view.size(), ints.size() * sizeof(int));
}

BOOST_AUTO_TEST_CASE(BufferViewViewsConstStdVector) {
	const auto ints = std::vector<int>{ 1, 2, 3 };

	auto view = viewBuffer(ints);

	static_assert(std::is_same_v<decltype(view)::ByteType, const Byte>);
	BOOST_CHECK_EQUAL(view.data(), reinterpret_cast<const Byte*>(&ints[0]));
	BOOST_CHECK_EQUAL(view.size(), ints.size() * sizeof(int));
}

BOOST_AUTO_TEST_CASE(BufferViewViewsStdString) {
	auto chars = "qwerty"s;

	auto view = viewBuffer(chars);

	static_assert(std::is_same_v<decltype(view)::ByteType, Byte>);
	BOOST_CHECK_EQUAL(view.data(), reinterpret_cast<Byte*>(&chars[0]));
	BOOST_CHECK_EQUAL(view.size(), chars.size() * sizeof(char));
}

BOOST_AUTO_TEST_CASE(BufferViewViewsConstStdString) {
	const auto chars = "qwerty"s;

	auto view = viewBuffer(chars);

	static_assert(std::is_same_v<decltype(view)::ByteType, const Byte>);
	BOOST_CHECK_EQUAL(view.data(), reinterpret_cast<const Byte*>(&chars[0]));
	BOOST_CHECK_EQUAL(view.size(), chars.size() * sizeof(char));
}

BOOST_AUTO_TEST_CASE(ConstBufferViewConstructibleFromNonConst) {
	auto chars = "qwerty"s;

	auto view = viewBuffer(chars);
	auto constView = ConstBufferView(view);

	static_assert(std::is_same_v<decltype(view)::ByteType, Byte>);
	static_assert(std::is_same_v<decltype(constView)::ByteType, const Byte>);
}

BOOST_AUTO_TEST_SUITE_END(/* MemoryTestSuite */);

} // anonymous namespace
