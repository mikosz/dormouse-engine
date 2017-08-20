#ifndef _DORMOUSEENGINE_SYSTEM_WINDOWS_COMWRAPPER_HPP_
#define _DORMOUSEENGINE_SYSTEM_WINDOWS_COMWRAPPER_HPP_

#include <cassert>
#include <type_traits>

namespace dormouse_engine::system::windows {

template <class T>
class COMWrapper {
public:

	COMWrapper() = default;

	COMWrapper(T* comObject) :
		comObject_(comObject)
	{
	}

	template <class CompatibleT>
	COMWrapper(const COMWrapper<CompatibleT>& other) :
		comObject_(other.get())
	{
		if (comObject_) {
			comObject_->AddRef();
		}
	}

	COMWrapper(const COMWrapper& other) :
		comObject_(other.comObject_)
	{
		if (comObject_) {
			comObject_->AddRef();
		}
	}

	COMWrapper(COMWrapper&& other) :
		COMWrapper()
	{
		swap(other);
	}

	~COMWrapper() {
		reset();
	}

	explicit operator bool() const {
		return comObject_ != nullptr;
	}

	COMWrapper& operator=(COMWrapper other) {
		swap(other);
		return *this;
	}

	T* operator->() const {
		assert(comObject_ != nullptr);
		return comObject_;
	}

	T& operator*() const {
		assert(comObject_ != nullptr);
		return *comObject_;
	}

	operator T*() const {
		return get();
	}

	void reset() {
		if (comObject_) {
			comObject_->Release();
			comObject_ = 0;
		}
	}

	void reset(T* comObject) {
		reset();
		comObject_ = comObject;
	}

	void swap(COMWrapper& other) {
		std::swap(comObject_, other.comObject_);
	}

	T*& get() { // TODO: WTF? Texture2D::initialise crashes when doing &get()
		return comObject_;
	}

	T* get() const {
		return comObject_;
	}

private:

	T* comObject_ = nullptr;

};

template <class T>
void swap(COMWrapper<T> lhs, COMWrapper<T> rhs) {
	lhs.swap(rhs);
}

} // namespace dormouse_engine::system::windows

#endif /* _DORMOUSEENGINE_SYSTEM_WINDOWS_COMWRAPPER_HPP_ */
