#pragma once
#include "enumerator.h"

#include <iostream>

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
		
		inline T* Find(bool_func func) {
			for (TEnumerator<T> i = begin(), e = end(); i != end(); ++i) {
				if (func(*i)) return i.Get();
			}

			return nullptr;
		}
	};
}