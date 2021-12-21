#pragma once

#include <string>
#include <ostream>

using type_value = std::string;
using type_key = int;

/// <summary>
/// HashItem is our stored key and value pair
/// </summary>
class HashItem
{
private:
	type_key key;
	type_value value;

public:
	HashItem(type_key key, type_value value) : key(key), value(value) {};
	HashItem(const HashItem& item);
	virtual ~HashItem() = default;

	type_value get_value() const  { return value; };/* const inline */
	type_key get_key() const { return key; }; /* const inline */
	void set_value(type_value value) { this->value = value; }

	HashItem& operator=(const HashItem& other);

	friend bool operator==(const HashItem& left_i, const HashItem& right_i);
	

	friend bool  operator!=(const HashItem& left_i, const HashItem& right_i) /*  inline */
	{
		return !(left_i == right_i);
	};

	friend std::ostream& operator<<(std::ostream& os, const HashItem& i);
	friend void swap(HashItem& i1, HashItem& i2);
};

