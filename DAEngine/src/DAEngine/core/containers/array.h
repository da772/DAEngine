#pragma once
#include "DAEngine/core/core.h"
#include "DAEngine/core/memory/memory.h"
#include "DAEngine/core/memory/global_allocator.h"
#include "enumerable.h"
#include <initializer_list>


namespace da::core::containers {

	template<typename T, typename _Allocator = memory::CGlobalAllocator>
	class TArray : public IEnumerable<T> {
	public:
		inline TArray() : m_size(0), m_ptr(nullptr) {

		}

		inline TArray(size_t size) { 
			ASSERT(size);
			resize(size);
		};

		inline TArray(int size) { 
			ASSERT(size);
			resize(size);
		};

		inline TArray(size_t size, T value) {
			ASSERT(size);
			resize(size);
			ASSERT(m_ptr);
			for (size_t i = 0; i < size; i++) {
				new(&m_ptr[i]) T;
				m_ptr[i] = value;
			}
		};

		inline TArray(const std::initializer_list<T>& l) {
			m_size = l.size();
			ASSERT(m_size);
			m_ptr = (T*)m_allocator.allocate(m_size*sizeof(T));
			size_t cnt = 0;
			for (const T& i : l) {
				new(&m_ptr[cnt]) T;
				m_ptr[cnt++] = i;
			}
		}

		inline TArray(const IEnumerable<T>& e) {
			resize(e.size());
			size_t c = 0;
			for (const T& i : e) {
				new(&m_ptr[c]) T;
				m_ptr[c++] = i;
			}
		}

		// Copy
		inline TArray(const TArray<T, _Allocator>& arr) {
			// Copy
			m_size = arr.m_size;
			ASSERT(m_size);
			m_ptr = (T*)m_allocator.allocate(m_size * sizeof(T));
			ASSERT(m_ptr);
			memcpy(m_ptr, arr.m_ptr, sizeof(T) * m_size);
		}

		// Move
		inline TArray(TArray<T, _Allocator>&& other) {
			m_ptr = other.m_ptr;
			m_size = other.m_size;
			other.m_ptr = nullptr;
			other.m_size = 0;
		}

		inline ~TArray() {
			clear();
		}

		inline void clear() { 
			if (!m_ptr) return;
			m_allocator.deallocate(m_ptr);
			m_ptr = nullptr;
			m_size = 0;
		};

		inline void resize(const size_t& n) {
			ASSERT(n);
			if (n == m_size) return;
			if (m_ptr) {
				m_ptr = (T*)m_allocator.reallocate(m_ptr, n * sizeof(T));
				ASSERT(m_ptr);
				if (n > m_size) for (size_t i = m_size; i < n; i++) new(&m_ptr[i]) T;
			}
			else {
				m_ptr = (T*)m_allocator.allocate(n * sizeof(T));
				ASSERT(m_ptr);
				for (size_t i = 0; i < n;  i++) new(&m_ptr[i]) T;
			}
			m_size = n;
		}

		inline virtual size_t size() const override {
			return m_size;
		}

		inline T* data() const { return m_ptr; }

		inline const TEnumerator<T> begin() const override {
			ASSERT(m_ptr);
			return TEnumerator<T>(&m_ptr[0]);
		};

		inline const TEnumerator<T> end() const override {
			ASSERT(m_ptr);
			return TEnumerator<T>(&m_ptr[m_size]);
		}

		inline TEnumerator<T> begin() override {
			ASSERT(m_ptr);
			return TEnumerator<T>(&m_ptr[0]);
		};

		inline TEnumerator<T> end() override {
			ASSERT(m_ptr);
			return TEnumerator<T>(&m_ptr[m_size]);
		}

		inline const T& operator [](const size_t& i) const {
			ASSERT(i < m_size);
			return m_ptr[i];
		};

		inline T& operator [](const size_t& i) { 
			ASSERT(i < m_size);
			return m_ptr[i];  
		};

		// Copy
		inline TArray<T, _Allocator>& operator=(const TArray<T, _Allocator>& other)
		{
			if (m_ptr) clear();

			m_size = other.m_size;
			ASSERT(m_size);
			m_ptr = (T*)m_allocator.allocate(m_size * sizeof(T));
			ASSERT(m_ptr);
			memcpy(m_ptr, other.m_ptr, sizeof(T) * m_size);
			
			return *this;
		}

		// Move
		inline TArray<T, _Allocator>& operator=(TArray<T, _Allocator>&& rhs) noexcept
		{
			if (m_ptr) clear();
			m_ptr = rhs.m_ptr;
			m_size = rhs.m_size;
			rhs.m_ptr = nullptr;
			rhs.m_size = 0;
			return *this;
		}

		inline bool operator==(const TArray<T, _Allocator>& rhs) const {
			if (m_size != rhs.m_size) return false;
			if (m_ptr == rhs.m_ptr) return true;
			if (!m_ptr) return false;
			for (size_t i = 0; i < m_size; i++) {
				if (m_ptr[i] != rhs.m_ptr[i]) return false;
			}

			return true;
		}

		inline bool operator==(const IEnumerable<T>& rhs) const {
			if (m_size != rhs.size()) return false;
			if (m_ptr == rhs.get()) return true;
			if (!m_ptr) return false;
			for (size_t i = 0; i < m_size; i++) {
				if (m_ptr[i] != rhs.get()[i]) return false;
			}

			return true;
		}

		inline bool operator!=(const IEnumerable<T>& rhs) const {
			if (m_size != rhs.size()) return true;
			if (m_ptr == rhs.get()) return false;
			if (!m_ptr) return true;
			for (size_t i = 0; i < m_size; i++) {
				if (m_ptr[i] != rhs.get()[i]) return true;
			}

			return true;
		}

		inline bool operator!=(const TArray<T, _Allocator>& rhs) const {
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
		_Allocator m_allocator;

	};

}
