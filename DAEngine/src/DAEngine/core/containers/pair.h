#pragma once
namespace da::core::containers {

	template<typename K, typename V>
	class TPair {
	public:
		inline TPair(K key, V val) : Key(key), Value(val) {

		}

		inline TPair(const TPair<K, V>& other) {
			Key = other.Key;
			Value = other.Value;
		}

		inline TPair(TPair<K, V>&& other) {
			Key = other.Key;
			Value = other.Value;
		}

		// Copy
		inline TPair<K, V>& operator=(const TPair<K, V>& other)
		{
			Key = other.Key;
			Value = other.Value;
			return *this;
		}

		// Move
		inline TPair<K, V>& operator=(TPair<K, V>&& rhs)
		{
			Key = rhs.Key;
			Value = rhs.Value;
			return *this;
		}

		inline bool operator==(const TPair<K, V>& rhs) const {
			return Key == rhs.Key && Value == rhs.Value;
		}

		inline bool operator!=(const TPair<K, V>& rhs) const {
			return Key != rhs.Key || Value != rhs.Value;
		}

		K Key;
		V Value;
	};
}
