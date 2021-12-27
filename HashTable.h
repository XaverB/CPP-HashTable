#pragma once
#include <memory>
#include <string>
#include <iostream>
#include <functional>

#include "HashTableNodeProxy.h"
#include "HashItemNode.h"
#include "HashItem.h"

using std::count;
using std::endl;
using std::count;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::function;


template <typename K, typename V>
class HashTable
{
	using type_key = K;
	using type_value = V;

private:
	const float default_load_factor = 0.75;
	int size;
	int element_count;
	std::unique_ptr<HashItemNode<K, V>* []> data;

	void init_hashtable()
	{
		data = make_unique<HashItemNode<K, V>* []>(size);
		for (int i = 0; i < size; i++) {
			data[i] = nullptr;
		}
	}

	void rehash_if_necessary()
	{
		auto load_factor = (1.0 * element_count) / size;
		if (load_factor > default_load_factor)
			rehash();
	}

	void rehash()
	{
		HashTable<K, V> temp{ size * 2 };

		// copy each entry from us to the temp hashtable
		for (int i = 0; i < size; i++) {
			auto node = data[i];
			while (node != nullptr) {
				auto item = node->getValue();
				if (item != nullptr) {
					temp.put(item->get_key(), item->get_value());
				}
				node = node->getNext();
			}
		}

		// this will trigger move assignment
		*this = temp;
	}

	type_key compute_hash(type_key key)
	{
		auto hash = std::hash<K>()(key);
		return hash % size;
	}

public:
	HashTable(int size) : size(size), data(nullptr), element_count(0)
	{
	}

	HashTable(const HashTable<K, V>& h)
	{
		size = h.size;
		element_count = h.element_count;

		init_hashtable();

		for (int i = 0; i < size; i++) {
			data[i] = new HashItemNode<K, V>{ *h.data[i] };
		}
	}

	HashTable(HashTable<K, V>&& h)  noexcept : size(0), data(nullptr), element_count(0)
	{
		// this works, because we implemented the move assignment operator
		// just convert h to rvl -> will use move assignment instead of assignment
		*this = std::move(h);
	}

	void put(type_key key, type_value value)
	{
		if (data == nullptr)
			init_hashtable();

		// maybe the hashitem should compute the has value
		auto hash = compute_hash(key);

		HashItem<K, V>* item_to_put = new HashItem<K, V>{ key, value };

		bool hash_exists = data[hash] != nullptr;
		// the cash exists -> we need to check, if the element is already in there
		// if not, we must insert it in chaining mode
		if (!hash_exists)
		{
			data[hash] = new HashItemNode<K, V>{ item_to_put };
			element_count++;
		}
		else {

			// search predicate
			const function<bool(HashItem<K, V>* item)> predicate = [&item_to_put](HashItem<K, V>* item) -> bool {
				return item == nullptr ? false : item_to_put->get_key() == item->get_key();
			};

			auto entry = data[hash];
			if (entry->exists(predicate)) {
				//auto desired_entry = get(key); 
				auto desired_entry = entry->get(key); // TODO: improve. we don't need to hash again etc.
				desired_entry->set_value(value);
				return;
			}
			else {
				element_count++;
				entry->add(item_to_put);
			}
		}
		rehash_if_necessary();
	}

	type_value get(type_key key)
	{
		auto hash = compute_hash(key);
		auto entry = data[hash];
		if (entry == nullptr)
			return type_value{};

		auto item = entry->get(key);
		return item == nullptr ? type_value{} : item->get_value();
	}

	void clear()
	{
		init_hashtable(); // old memory will be freed. thank you smart pointer!
	}

	bool contains(type_key key)
	{
		auto hash = compute_hash(key);
		auto entry = data[hash];

		if (entry == nullptr)
			return false;

		// we need to use std::functional if we want to be able to pass predicates with captures
		const function<bool(HashItem<K, V>* item)> predicate = [&key](HashItem<K, V>* item) -> bool {
			return item == nullptr ? false : item->get_key() == key;
		};

		return entry == nullptr ? false : entry->exists(predicate);
	}

	bool contains_value(type_value value)
	{
		const function<bool(HashItem<K, V>* item)> predicate = [&value](HashItem<K, V>* item) -> bool {
			return item == nullptr ? false : item->get_value() == value;
		};

		for (int i = 0; i < size; i++) {
			auto node = data[i];

			if (node == nullptr)
				continue;

			if (node->exists(predicate))
				return true;
		}

		return false;
	}

	HashTable<K, V>& operator=(const HashTable<K, V>& h)
	{
		HashTable<K, V> temp(h);
		swap(*this, temp);
		return *this;
	}

	HashTable<K, V>& operator=(HashTable<K, V>&& h) noexcept
	{
		if (this != &h)
		{
			size = h.size;
			element_count = h.element_count;
			data = std::move(h.data);

			h.data = nullptr;
			h.size = 0;
			h.element_count = 0;
		}
		return *this;
	}

	HashTableNodeProxy<K, V> operator[](type_key key)
	{
		return HashTableNodeProxy<K, V>{ this, key };
	}

	template<typename K, typename V>
	friend std::ostream& operator<<(std::ostream& os, const HashTable<K, V>& t)
	{
		bool is_first = true;

		for (int i = 0; i < t.size; i++) {
			auto node = t.data[i];
			if (node == nullptr)
				continue;

			if (!is_first && &(*node->getValue()) != nullptr) {
				os << ", ";
			}
			os << *node;
			if (&(*node->getValue()) != nullptr)
				is_first = false;
		}

		return os;
	}

	template<typename K, typename V>
	friend bool operator==(const HashTable<K, V>& h1, const HashTable<K, V>& h2)
	{
		if (h1.size != h2.size)
			return false;

		if (h1.data == nullptr || h2.data == nullptr)
			return false;

		for (int i = 0; i < h1.size; i++) {
			auto node1 = h1.data[i];
			auto node2 = h2.data[i];
			if (*node1 != *node2)
				return false;
		}

		return true;
	}

	template<typename K, typename V>
	friend bool operator!=(const HashTable<K, V>& h1, const HashTable<K, V>& h2)
	{
		return !(h1 == h2);
	}

	template<typename K, typename V>
	friend void swap(HashTable<K, V>& h1, HashTable<K, V>& h2)
	{
		using std::swap;
		swap(h1.size, h2.size);
		swap(h1.data, h2.data);
		swap(h1.element_count, h2.element_count);
	}
};

