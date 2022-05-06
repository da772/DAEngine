#pragma once
#include "enumerator.h"

namespace da::core::containers {

	template <typename T>
	class IEnumerable {
	private:
		typedef bool (*bool_func)(const T&);
	public:
		virtual TEnumerator<T> begin() const = 0;
		virtual TEnumerator<T> end() const = 0;

		inline bool Contains(const T& x) const {
			TEnumerator<T> i = begin(), e = end();
			for (const T& i : *this) {
				if (i == x) return true;
			}
			
			return false;
		}

		inline bool Contains(bool_func func) const {
			for (const T& i : *this) {
				if (func(i)) return true;
			}
		
			return false;
		}
		
		inline TEnumerator<T> Find(bool_func func) const {
			for (TEnumerator<T> i = begin(), e = end(); i != e; ++i) {
				if (func(*i)) return i;
			}
			return end();
		}

		inline TEnumerator<T> Find(const T& x) const {
			for (TEnumerator<T> i = begin(), e = end(); i != e; ++i) {
				if (x == *i) return i;
			}
			return end();
		}

		inline size_t Size() const {
			return ((uintptr_t)end().Get() - (uintptr_t)begin().Get())/sizeof(T);
		}

	};
}