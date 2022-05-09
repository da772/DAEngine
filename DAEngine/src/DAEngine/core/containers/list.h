#pragma once
#include "DAEngine/core/core.h"
#include "DAEngine/core/maths/maths.h"
#include "enumerable.h"

namespace da::core::containers {

	template<typename T>
	class TList : public IEnumerable<T>
	{
	public:
		TList() {

		}

		TList(const IEnumerable<T>& n) {
			resize(n.size());
			size_t c = 0;
			for (const T& i : n) {
				m_ptr[c++] = i;
			}
		}

		TList(const size_t& size) : m_size(size) {
			resize(size);
		}

		inline void push(const T& x) {
			resize(m_size + 1);
			m_ptr[m_size - 1] = x;
		}

		inline void push(T&& x) {
			resize(m_size + 1);
			m_ptr[m_size - 1] = T(x);
		}

		inline void pop() {
			ASSERT(m_size);
			resize(m_size - 1);
		}

		inline void insert(const T& o, const size_t& n) {
			ASSERT(n < m_size);
			resize(m_size + 1);

			memcpy(&m_ptr[n + 1], &m_ptr[n], sizeof(T) * (m_size-n-1) );
			m_ptr[n] = o;
		}

		inline void insert(T&& o, const size_t& n) {
			ASSERT(n < m_size);
			resize(m_size + 1);

			memcpy(&m_ptr[n + 1], &m_ptr[n], sizeof(T) * (m_size - n - 1));
			m_ptr[n] = T(o);
		}

		inline void remove(const TEnumerator<T>& it) {
			if (it == &m_ptr[m_size]) {
				pop();
				return;
			}

			TEnumerator<int> nxt(&it.get()[1]);
			memcpy(it.get(), nxt.get(), ((uintptr_t)end().get() - (uintptr_t)nxt.get()));
			resize(m_size - 1);
		}

		inline void remove(const size_t& i) {
			remove(TEnumerator<int>(&m_ptr[i]));
		}

		inline size_t size() const {
			return m_size;
		}

		inline void resize(const size_t& n) {
			if (!n) {
				clear();
				return;
			}

			ASSERT(n);

			if (n < m_heapSize && m_heapSize < (n << 2) ) {
				m_size = n;
				return;
			};

			size_t newSize = n << 1;
			
			if (m_ptr) {
				T* ptr = (T*)realloc(m_ptr, sizeof(T) * newSize);
				ASSERT(ptr);
				m_ptr = ptr;
			}
			else m_ptr = new T[newSize];
			ASSERT(m_ptr);
			m_heapSize = newSize;
			m_size = n;
		}

		inline void clear() {
			if (!m_ptr) return;
			m_size = 0;
			m_heapSize = 0;
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
			if (m_ptr) clear();

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
			if (m_ptr) clear();
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
		size_t m_heapSize = 0;
		T* m_ptr = nullptr;

	};
}