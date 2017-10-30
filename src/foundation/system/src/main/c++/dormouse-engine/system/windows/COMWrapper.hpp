#ifndef _DORMOUSEENGINE_SYSTEM_WINDOWS_COMWRAPPER_HPP_
#define _DORMOUSEENGINE_SYSTEM_WINDOWS_COMWRAPPER_HPP_

#include <cassert>
#include <type_traits>

namespace dormouse_engine::system::windows {

template <class T>
class COMWrapper {
public:

	COMWrapper() = default;

	COMWrapper(T* comObject) noexcept :
		comObject_(comObject)
	{
	}

	template <class CompatibleT>
	COMWrapper(const COMWrapper<CompatibleT>& other) noexcept :
		comObject_(other.get())
	{
		if (comObject_) {
			comObject_->AddRef();
		}
	}

	COMWrapper(const COMWrapper& other) noexcept :
		comObject_(other.comObject_)
	{
		if (comObject_) {
			comObject_->AddRef();
		}
	}

	COMWrapper(COMWrapper&& other) noexcept :
		COMWrapper()
	{
		swap(other);
	}

	~COMWrapper() noexcept {
		reset();
	}

	explicit operator bool() const noexcept {
		return comObject_ != nullptr;
	}

	COMWrapper& operator=(COMWrapper other) noexcept {
		swap(other);
		return *this;
	}

	T* operator->() const noexcept {
		assert(comObject_ != nullptr);
		return comObject_;
	}

	T& operator*() const noexcept {
		assert(comObject_ != nullptr);
		return *comObject_;
	}

	operator T*() const noexcept {
		return get();
	}

	void reset() noexcept {
		if (comObject_) {
			comObject_->Release();
			comObject_ = 0;
		}
	}

	void reset(T* comObject) noexcept {
		reset();
		comObject_ = comObject;
	}

	void swap(COMWrapper& other) noexcept {
		std::swap(comObject_, other.comObject_);
	}

	T*& get() noexcept { // TODO: WTF? Texture2D::initialise crashes when doing &get()
		return comObject_;
	}

	T* get() const noexcept {
		return comObject_;
	}

private:

	T* comObject_ = nullptr;

	friend bool operator==(const COMWrapper& lhs, const COMWrapper& rhs) {
		return lhs.comObject_ == rhs.comObject_;
	}

};

template <class T>
void swap(COMWrapper<T> lhs, COMWrapper<T> rhs) {
	lhs.swap(rhs);
}

} // namespace dormouse_engine::system::windows

#endif /* _DORMOUSEENGINE_SYSTEM_WINDOWS_COMWRAPPER_HPP_ */
