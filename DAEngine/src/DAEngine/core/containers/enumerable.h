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
		virtual size_t size() const = 0;

		inline bool contains(const T& x) const {
			TEnumerator<T> i = begin(), e = end();
			for (const T& i : *this) {
				if (i == x) return true;
			}
			
			return false;
		}

		inline bool contains(bool_func func) const {
			for (const T& i : *this) {
				if (func(i)) return true;
			}
		
			return false;
		}
		
		inline TEnumerator<T> find(bool_func func) const {
			for (TEnumerator<T> i = begin(), e = end(); i != e; ++i) {
				if (func(*i)) return i;
			}
			return end();
		}

		inline TEnumerator<T> find(const T& x) const {
			for (TEnumerator<T> i = begin(), e = end(); i != e; ++i) {
				if (x == *i) return i;
			}
			return end();
		}

		inline bool isEmpty() const {
			return size() == 0;
		}
	};
}