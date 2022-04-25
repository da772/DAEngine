#pragma once
#include "DAEngine/core/core.h"
#include "enumerable.h"

namespace da::core::containers {

	template<typename T>
	class TArray : public IEnumerable<T> {
	public:
		inline TArray() : m_size(0), m_ptr(nullptr) {

		}

		inline TArray(const IEnumerable<T>& e) {
			Resize();
			size_t c = 0;
			for (const T& i : e) {
				m_ptr[c++] = i;
			}
		}

		// Copy
		inline TArray(const TArray<T>& arr) {
			// Copy
			m_size = arr.m_size;
			ASSERT(m_size);
			m_ptr = new T[m_size];
			ASSERT(m_ptr);
			memcpy(m_ptr, arr.m_ptr, sizeof(T) * m_size);
		}

		// Move
		inline TArray(TArray<T>&& other) {
			if (m_ptr) Clear();
			m_ptr = other.m_ptr;
			m_size = other.m_size;
			other.m_ptr = nullptr;
			other.m_size = 0;
		}

		inline ~TArray() {
			Clear();
		}

		inline TArray(size_t size) { 
			m_size = size;
			ASSERT(m_size);
			m_ptr = new T[m_size];
			ASSERT(m_ptr);
		};

		inline TArray(size_t size, T value) {
			m_size = size;
			ASSERT(m_size);
			m_ptr = new T[m_size];
			ASSERT(m_ptr);
			for (size_t i = 0; i < m_size; i++) m_ptr[i] = value;
		};

		inline void Clear() { 
			if (!m_ptr) return;
			delete[] m_ptr; 
			m_ptr = nullptr;
			m_size = 0;
		};

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

		inline T* Data() const { return m_ptr; }

		inline TEnumerator<T> begin() const override {
			ASSERT(m_ptr);
			return TEnumerator<T>(&m_ptr[0]);
		};

		inline TEnumerator<T> end() const override {
			ASSERT(m_ptr);
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
		inline TArray<T>& operator=(const TArray<T>& other)
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
		inline TArray<T>& operator=(TArray<T>&& rhs) noexcept
		{
			if (m_ptr) Clear();
			m_ptr = rhs.m_ptr;
			m_size = rhs.m_size;
			rhs.m_ptr = nullptr;
			rhs.m_size = 0;
			return *this;
		}

		inline bool operator==(const TArray<T>& rhs) const {
			if (m_size != rhs.m_size) return false;
			if (m_ptr == rhs.m_ptr) return true;
			if (!m_ptr) return false;
			for (size_t i = 0; i < m_size; i++) {
				if (m_ptr[i] != rhs.m_ptr[i]) return false;
			}

			return true;
		}

		inline bool operator==(const IEnumerable<T>& rhs) const {
			if (m_size != rhs.Size()) return false;
			if (m_ptr == rhs.Get()) return true;
			if (!m_ptr) return false;
			for (size_t i = 0; i < m_size; i++) {
				if (m_ptr[i] != rhs.Get()[i]) return false;
			}

			return true;
		}

		inline bool operator!=(const IEnumerable<T>& rhs) const {
			if (m_size != rhs.Size()) return true;
			if (m_ptr == rhs.Get()) return false;
			if (!m_ptr) return true;
			for (size_t i = 0; i < m_size; i++) {
				if (m_ptr[i] != rhs.Get()[i]) return true;
			}

			return true;
		}

		inline bool operator!=(const TArray<T>& rhs) const {
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