#pragma once
#include <string>
#include <memory>
#include <iostream>

#include "HashItemNode.h"
#include "HashItem.h"
#include "HashTableNodeProxy.h"

using type_key = int;
using type_value = std::string;

class HashTable
{
private:
	const float default_load_factor = 0.75;
	int size;
	int element_count;
	std::unique_ptr<HashItemNode*[]> data;

	void init_hashtable();
	type_key compute_hash(type_key key);
	void rehash_if_necessary();
	void rehash();

public:
	HashTable(int size);
	~HashTable();
	HashTable(const HashTable& h);
	HashTable(HashTable&& h) noexcept;

	void put(type_key key, type_value value);
	type_value get(type_key key);
	void clear();
	bool contains(type_key key);
	bool contains_value(type_value value);
	
	HashTable& operator=(const HashTable& h);
	HashTable& operator=(HashTable&& h) noexcept;

	/// <summary>
	/// Index operator must return a reference as array element can be put on left side
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	HashTableNodeProxy operator[](type_key key);

	friend std::ostream& operator<<(std::ostream& os, const HashTable& t);
	friend bool operator==(const HashTable& h1, const HashTable& h2);
	friend bool operator!=(const HashTable& h1, const HashTable& h2);
	friend void swap(HashTable& h1, HashTable& h2);
};

