#pragma once
#include <string>
#include <memory>
#include <iostream>

#include "HashItemNode.h"
#include "HashItem.h"

using type_key = int;
using type_value = std::string;

class HashTable
{
private:
	int max_filling_level_percentage = 50;
	int size;
	std::unique_ptr<HashItemNode*[]> data;

	void init_hashtable();
	void rehash();
	type_key compute_hash(type_key key);

public:
	HashTable(int size);
	~HashTable();

	// copy constructor
	HashTable(const HashTable& h);
	// move constructor
	HashTable(HashTable&& h) noexcept;

	void put(type_key key, type_value value);
	type_value get(type_key key);
	void clear();
	bool contains(type_key key);
	bool contains_value(type_value value);

	// assignment operator
	HashTable& operator=(const HashTable& h);
	// move assignment operator
	HashTable& operator=(HashTable&& h) noexcept;
	// output operator
	friend std::ostream& operator<<(std::ostream& os, const HashTable& t);
	friend bool operator==(const HashTable& h1, const HashTable& h2);
	friend bool operator!=(const HashTable& h1, const HashTable& h2);
	friend void swap(HashTable& h1, HashTable& h2);
};

