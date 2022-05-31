#pragma once

namespace da::core::containers {
	template <typename T>
	struct TEnumerator
	{
	public:
		TEnumerator(T* ptr) : m_ptr(ptr) {

		}

		inline T& operator*() const { return *m_ptr; }
		inline T* operator->() const { return m_ptr; }
		inline T* get() const { return m_ptr; }

		inline TEnumerator& operator++() { m_ptr++; return *this; }
		inline TEnumerator operator++(int) { TEnumerator tmp = *this; ++(*this); return tmp; }

		friend bool operator== (const TEnumerator& a, const TEnumerator& b) { return a.m_ptr == b.m_ptr; };
		friend bool operator!= (const TEnumerator& a, const TEnumerator& b) { return a.m_ptr != b.m_ptr; };

	private:
		T* m_ptr;
	};
}