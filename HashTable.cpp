#include "HashTable.h"
#include <memory>
#include <iostream>
#include <functional>

#include "HashItemNode.h"
#include "HashItem.h"

using std::count;
using std::endl;
using std::count;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::function;

// -- -- -- private -- -- --

void HashTable::init_hashtable()
{
	data = make_unique<HashItemNode* []>(size);
	// maybe we don't need this manual nullptr initialization
	for (int i = 0; i < size; i++) {
		data[i] = nullptr;
	}
	// maybe add checks if memory was allocated?
}

void HashTable::rehash_if_necessary()
{
	auto load_factor = (1.0 * element_count) / size;
	if (load_factor > default_load_factor)
		rehash();
}

void HashTable::rehash()
{
	HashTable temp{ size * 2 };

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

type_key HashTable::compute_hash(type_key key)
{
	// TODO improve hashing algorithmus
	return key % size;
}

// -- -- -- public -- -- --

HashTable::HashTable(int size) : size(size), data(nullptr), element_count(0)
{
}

HashTable::~HashTable()
{
	// nothing to do thanks to smart pointer!
}

HashTable::HashTable(const HashTable& h)
{
	size = h.size;
	element_count = h.element_count;
	init_hashtable();

	for (int i = 0; i < size; i++) {
		data[i] = new HashItemNode{ *h.data[i] };
	}

}

// move constructor
HashTable::HashTable(HashTable&& h)  noexcept : size(0), data(nullptr), element_count(0)
{
	// this works, because we implemented the move assignment operator
	// just convert h to rvl -> will use move assignment instead of assignment
	*this = std::move(h);
}

void HashTable::put(type_key key, type_value value)
{
	if (data == nullptr)
		init_hashtable();

	// maybe the hashitem should compute the has value
	auto hash = compute_hash(key);

	HashItem* item_to_put = new HashItem{ key, value };

	bool hash_exists = data[hash] != nullptr;
	// the cash exists -> we need to check, if the element is already in there
	// if not, we must insert it in chaining mode
	if (!hash_exists)
	{
		data[hash] = new HashItemNode{ item_to_put };
		element_count++;
	}
	else {

		// search predicate
		const function<bool(HashItem* item)> predicate = [&item_to_put](HashItem* item) -> bool {
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

type_value HashTable::get(type_key key)
{
	auto hash = compute_hash(key);
	auto entry = data[hash];
	if (entry == nullptr)
		return type_value{};

	auto item = entry->get(key);
	return item == nullptr ? type_value{} : item->get_value();
}

void HashTable::clear()
{
	init_hashtable(); // old memory will be freed. thank you smart pointer!
}

bool HashTable::contains(type_key key)
{
	auto hash = compute_hash(key);
	auto entry = data[hash];

	if (entry == nullptr)
		return false;

	// we need to use std::functional if we want to be able to pass predicates with captures
	const function<bool(HashItem* item)> predicate = [&key](HashItem* item) -> bool {
		return item == nullptr ? false : item->get_key() == key;
	};

	return entry == nullptr ? false : entry->exists(predicate);
}

bool HashTable::contains_value(type_value value)
{
	const function<bool(HashItem* item)> predicate = [&value](HashItem* item) -> bool {
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

// move assignment
HashTable& HashTable::operator=(HashTable&& h) noexcept
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

std::ostream& operator<<(std::ostream& os, const HashTable& t)
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
		if(&(*node->getValue()) != nullptr)
			is_first = false;
	}

	return os;
}

bool operator==(const HashTable& h1, const HashTable& h2)
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

bool operator!=(const HashTable& h1, const HashTable& h2)
{
	return !(h1 == h2);
}

HashTable& HashTable::operator=(const HashTable& h)
{
	HashTable temp(h);
	swap(*this, temp);
	return *this;
}

void swap(HashTable& h1, HashTable& h2)
{
	using std::swap;
	swap(h1.size, h2.size);
	swap(h1.data, h2.data);
	swap(h1.element_count, h2.element_count);
}
