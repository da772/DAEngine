#pragma once
#include "DAEngine/core/core.h"
#include "enumerable.h"

namespace da::core::containers {

	template<typename T>
	class TList : public IEnumerable<T>
	{
	public:
		TList() {

		}

		TList(const IEnumerable<T>& n) {
			Resize(n.Size());
			size_t c = 0;
			for (const T& i : n) {
				m_ptr[c++] = i;
			}
		}

		TList(const size_t& size) : m_size(size) {
			Resize(size);
		}

		inline void Push(const T& x) {
			Resize(m_size + 1);
			m_ptr[m_size - 1] = x;
		}

		inline void Pop() {
			ASSERT(m_size);
			Resize(m_size - 1);
		}

		inline void Insert(const T& o, const size_t& n) {
			ASSERT(n < m_size);
			Resize(m_size + 1);

			memcpy(&m_ptr[n + 1], &m_ptr[n], sizeof(T) * (m_size-n-1) );
			m_ptr[n] = o;
		}

		inline void Remove(const TEnumerator<T>& it) {
			if (it == &m_ptr[m_size]) {
				Pop();
				return;
			}

			TEnumerator<int> nxt(&it.Get()[1]);
			memcpy(it.Get(), nxt.Get(), ((uintptr_t)end().Get() - (uintptr_t)nxt.Get()));
			Resize(m_size - 1);
		}

		inline void Remove(const size_t& i) {
			Remove(TEnumerator<int>(&m_ptr[i]));
		}

		inline void Resize(const size_t& n) {
			ASSERT(n);
			if (n == m_size) return;
			T* ptr = new T[n];
			if (m_ptr)
				memcpy(ptr, m_ptr, sizeof(T) * (n > m_size ? m_size : n));
			Clear();
			m_ptr = ptr;
			m_size = n;
		}

		inline void Clear() {
			if (!m_ptr) return;
			m_size = 0;
			delete[] m_ptr;
			m_ptr = nullptr;
		}

		TEnumerator<T> begin() const override {
			return TEnumerator<T>(m_ptr);
		}

		TEnumerator<T> end() const override {
			return TEnumerator<T>(&m_ptr[m_size]);
		}

		inline T operator [](const size_t& i) const {
			ASSERT(i < m_size);
			return m_ptr[i];
		};
		inline T& operator [](const size_t& i) {
			ASSERT(i < m_size);
			return m_ptr[i];
		};

		// Copy
		inline TList<T>& operator=(const TList<T>& other)
		{
			if (m_ptr) Clear();

			m_size = other.m_size;
			ASSERT(m_size);
			m_ptr = new T[m_size];
			ASSERT(m_ptr);
			memcpy(m_ptr, other.m_ptr, sizeof(T) * m_size);

			return *this;
		}

		// Move
		inline TList<T>& operator=(TList<T>&& rhs) noexcept
		{
			if (m_ptr) Clear();
			m_ptr = rhs.m_ptr;
			m_size = rhs.m_size;
			rhs.m_ptr = nullptr;
			rhs.m_size = 0;
			return *this;
		}

		inline bool operator==(const TList<T>& rhs) const {
			if (m_size != rhs.m_size) return false;
			if (m_ptr == rhs.m_ptr) return true;
			if (!m_ptr) return false;
			for (size_t i = 0; i < m_size; i++) {
				if (m_ptr[i] != rhs.m_ptr[i]) return false;
			}

			return true;
		}

		inline bool operator!=(const TList<T>& rhs) const {
			if (m_size != rhs.m_size) return true;
			if (m_ptr == rhs.m_ptr) return false;
			if (!m_ptr) return true;
			for (size_t i = 0; i < m_size; i++) {
				if (m_ptr[i] != rhs.m_ptr[i]) return true;
			}

			return false;
		}

	private:
		size_t m_size = 0;
		T* m_ptr = nullptr;

	};
}