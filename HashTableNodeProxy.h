#pragma once

#include <string>

// template forward declaration
template <typename K, typename V>
class HashTable;

template <typename K, typename V>
class HashTableNodeProxy
{
private:
	HashTable<K, V>* table;
	K key;

public:
	HashTableNodeProxy(HashTable<K, V>* table, const K key) : table(table), key(key)
	{
	}

	const V& operator=(const V& value)
	{
		table->put(key, value);
		return table->get(key);
	}

	operator V() const
	{
		return table->get(key);
	}
};

