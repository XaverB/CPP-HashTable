#pragma once

#include <string>
#include <ostream>

using std::ostream;

/// <summary>
/// HashItem is our stored key and value pair
/// </summary>
/// 
template <typename K, typename V>
class HashItem
{
	using type_key = K;
	using type_value = V;

private:
	type_key key;
	type_value value;

public:
	HashItem(const HashItem<K, V>& item)
	{
		this->key = item.get_key();
		this->value = item.get_value();
	}

	HashItem(type_key key, type_value value) : key(key), value(value) {};

	HashItem<K, V>& operator=(const HashItem<K, V>& other)
	{
		HashItem<K, V> temp(other);
		swap(*this, temp);
		return *this;
	}

	type_value get_value() const { return value; };/* const inline */
	type_key get_key() const { return key; }; /* const inline */
	void set_value(type_value value) { this->value = value; }

	friend bool operator==(const HashItem<K, V>& left_i, const HashItem<K, V>& right_i)
	{
		if (&left_i == nullptr && &right_i == nullptr)
			return true;
		if (&left_i == nullptr || &right_i == nullptr)
			return false;

		return left_i.get_key() == right_i.get_key()
			&& left_i.get_value() == right_i.get_value();
	}

	friend std::ostream& operator<<(ostream& os, const HashItem<K, V>& i)
	{
		if (&i != nullptr)
			os << "[" << i.get_key() << ", " << i.get_value() << "]";
		return os;
	}

	friend void swap(HashItem<K, V>& i1, HashItem<K, V>& i2)
	{
		using std::swap;
		swap(i1.key, i2.key);
		swap(i1.value, i2.value);
	}


	friend bool  operator!=(const HashItem<K, V>& left_i, const HashItem<K, V>& right_i)
	{
		return !(left_i == right_i);
	};
};

