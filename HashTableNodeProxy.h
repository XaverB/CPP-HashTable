#pragma once

#include <string>

class HashTable;

using type_key = int;
using type_value = std::string;

class HashTableNodeProxy
{
private:
	HashTable* table;
	type_key key;

public:
	HashTableNodeProxy(HashTable* table, type_key key);

	const type_value& operator = (const type_value& value);
	operator type_value() const;

};

