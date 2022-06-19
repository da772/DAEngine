#pragma once
#include "daengine/core/core.h"
#include "daengine/core/maths/maths.h"
#include "enumerable.h"
#include <cstdlib>
#include <cstring>
#include <initializer_list>

namespace da::core::containers {

	template<typename T>
	class TList : public IEnumerable<T>
	{
	public:
		inline TList() {

		}

		inline TList(const T* arr, size_t n) {
			for (size_t i = 0; i < n; i++) {
				push(arr[i]);
			}
		}

		inline TList(const std::initializer_list<T>& l) {
			resize(l.size());
			size_t c = 0;
			for (const T& t : l) {
				m_ptr[c++] = t;
			}
		}

		inline TList(const IEnumerable<T>& n) {
			resize(n.size());
			size_t c = 0;
			for (const T& i : n) {
				m_ptr[c++] = i;
			}
		}

		inline TList(const TList& other) {
			if (!other.size()) return;
			resize(other.size());
			ASSERT(m_ptr);
			memcpy(m_ptr, other.m_ptr, sizeof(T) * other.size());
			m_heapSize = other.size();
			m_size = other.size();
		}

		inline TList(TList&& other)
		{
			resize(other.size());
			m_ptr = (T*)malloc(other.size() * sizeof(T));
			ASSERT(m_ptr);
			memcpy(m_ptr, other.m_ptr, sizeof(T) * other.size());
			m_heapSize = other.size();
			m_size = other.size();
			other.clear();
		}

		inline TList(const size_t& size) {
			ASSERT(size);
			resize(size);
		}

		inline ~TList()
		{
			clear();
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

			memcpy(&m_ptr[n + 1], &m_ptr[n], sizeof(T) * (m_size - n - 1));
			m_ptr[n] = o;
		}

		inline void insert(const IEnumerable<T>& e, const size_t& n) {
			ASSERT(e.size());
			ASSERT(n < m_size);
			resize(m_size + e.size());

			memcpy(&m_ptr[n + e.size()], &m_ptr[n], sizeof(T) * (size() - n - 1));

			TEnumerator<T> d = e.begin();

			for (size_t i = 0; i < e.size(); i++) {
				m_ptr[n + i] = *d;
				d++;
			}
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

			TEnumerator<T> nxt(&it.get()[1]);
			memcpy(it.get(), nxt.get(), ((uintptr_t)end().get() - (uintptr_t)nxt.get()));
			resize(m_size - 1);
		}

		inline void remove(const size_t& i) {
			remove(TEnumerator<int>(&m_ptr[i]));
		}

		inline size_t size() const override {
			return m_size;
		}

		inline void resize(const size_t& n) {
			if (!n) {
				clear();
				return;
			}

			ASSERT(n);

			if (n < m_heapSize && m_heapSize < (n << (m_shiftSize+1) )) {
				m_size = n;
				return;
			};

			size_t newSize = n << m_shiftSize;

			if (m_ptr) {
				T* ptr = (T*)realloc(m_ptr, sizeof(T) * newSize);
				ASSERT(ptr);
				m_ptr = ptr;
			}
			else m_ptr = (T*)malloc(newSize * sizeof(T));
			ASSERT(m_ptr);
			m_heapSize = newSize;
			m_size = n;
		}

		inline void clear() {
			if (!m_ptr) return;
			free(m_ptr);
			m_size = 0;
			m_heapSize = 0;
			m_ptr = nullptr;
		}

		inline virtual const TEnumerator<T> begin() const override {
			return TEnumerator<T>(m_ptr);
		}

		inline virtual const TEnumerator<T> end() const override {
			return TEnumerator<T>(&m_ptr[m_size]);
		}

		inline virtual TEnumerator<T> begin() override {
			return TEnumerator<T>(m_ptr);
		}

		inline virtual TEnumerator<T> end() override {
			return TEnumerator<T>(&m_ptr[m_size]);
		}

		inline T& operator [](const size_t& i) { /*strengthen with const*/
			ASSERT(i < m_size);
			return m_ptr[i];
		}

		inline const T& operator [](const size_t& i) const {
			ASSERT(i < m_size);
			return m_ptr[i];
		}

		// Copy
		inline TList<T>& operator=(const TList<T>& other)
		{
			if (m_ptr) clear();

			m_size = other.m_size;
			ASSERT(m_size);
			m_ptr = (T*)malloc(m_size * sizeof(T));
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

	protected:
		size_t m_size = 0;
		size_t m_heapSize = 0;
		T* m_ptr = nullptr;
        uint8_t m_shiftSize = 1;

	};
}
