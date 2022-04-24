#pragma once

template<typename Key, typename Value>
class TPair {
public:
	inline TPair(Key key, Value val) : Key(key), Value(val) {

	}

	inline TPair(const TPair<Key, Value>& other) {
		Key = other.Key;
		Value = other.Value;
	}
	
	inline TPair(TArray<Key, Value>&& other) {
		Key = other.Key;
		Value = other.Value;
	}

	// Copy
	inline TPair<Key, Value>& operator=(const TPair<Key,Value>& other)
	{
		Key = other.Key;
		Value = other.Value;
		return *this;
	}

	// Move
	inline TPair<Key, Value>& operator=(TArray<Key, Value>&& rhs)
	{
		Key = rhs.Key;
		Value = rhs.Value;
		return *this;
	}

	inline bool operator==(const TPair<Key,Value>& rhs) const {
		return Key == rhs.Key && Value == rhs.Value;
	}

	inline bool operator!=(const TPair<Key,Value>& rhs) const {
		return Key != rhs.Key || Value != rhs.Value;
	}

public:
	Key Key;
	Value Value;
};
