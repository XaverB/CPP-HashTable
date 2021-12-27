#pragma once

#include <functional>
#include <ostream>
#include "HashItem.h"

/// <summary>
/// We store HashItemNodes for chaining in case of collisions
/// </summary>
template <typename K, typename V>
class HashItemNode
{
private:
	HashItem<K, V>* value;
	HashItemNode<K, V>* next;

public:
	HashItemNode(HashItem<K, V>* item) : value(item), next(nullptr) {};

	HashItemNode<K, V>(const HashItemNode<K, V>& h) : next(nullptr), value(nullptr)
	{
		if (h != nullptr) {
			next = new HashItemNode<K, V>{ *h.getNext() };
			auto item = &(*h.getValue());
			value = item == nullptr ? nullptr : new HashItem<K, V>{ *h.getValue() };
		}
	}

	HashItem<K, V>* getValue() const { return value; }
	HashItemNode<K, V>* getNext() const { return next; } 
	void setNext(HashItemNode<K, V>* next) { this->next = next; }

	bool exists(const std::function<bool(HashItem<K, V>* item)> predicate)
	{
		auto node = this;
		while (node != nullptr) {
			if (predicate(node->getValue()))
				return true;
			node = node->getNext();
		}
		return false;
	}

	void add(HashItem<K, V>* item)
	{
		auto node = this;
		while (node->next != nullptr) {
			node = node->getNext();
		}

		HashItemNode<K, V>* new_node = new HashItemNode<K, V>{ item };
		node->setNext(new_node);
	}

	HashItem<K, V>* get(K key) const
	{
		auto node = this;
		while (node != nullptr) {
			auto item = node->getValue();
			if (item != nullptr && item->get_key() == key)
				return item;
			node = node->getNext();
		}
		return nullptr;
	}

	HashItemNode<K, V>& operator=(const HashItemNode<K, V>& other)
	{
		HashItemNode<K, V> temp(other);
		swap(*this, temp);
		return *this;
	}

	friend bool operator==(const HashItemNode<K, V>& left_h, const HashItemNode<K, V>& right_h)
	{
		auto node1 = &left_h;
		auto node2 = &right_h;

		if (node1 == nullptr && node2 == nullptr)
		{
			return true;
		}

		while (node1 != nullptr) {
			// this will check the pointers (to handle node2 nullptrs)
			if (node2 == nullptr)
				return false;

			auto item1 = node1->getValue();
			auto item2 = node2->getValue();

			// this conditions represents a true logical XOR operation
			if (!(item1 == nullptr) != !(item2 == nullptr))
				return false;

			node1 = node1->getNext();
			node2 = node2->getNext();
		}

		return true;
	}

	friend bool operator!=(const HashItemNode<K, V>& left_h, const HashItemNode<K, V>& right_h)
	{
		return !(left_h == right_h);
	};

	friend std::ostream& operator<<(std::ostream& os, const HashItemNode<K, V>& n)
	{
		auto node = &n;
		while (node != nullptr) {

			auto item = node->getValue();
			os << *item;
			node = node->getNext();
		}
		return os;
	}

	friend void swap(HashItemNode<K, V>& i1, HashItemNode<K, V>& i2)
	{
		using std::swap;
		swap(i1.next, i2.next);
		swap(i1.value, i2.value);
	}

	friend bool operator==(const HashItemNode<K, V>& left_i, const K& key)
	{
		return left_i.getValue()->get_key() == key;
	};

	friend bool operator!=(const HashItemNode<K, V>& left_i, const K& key)
	{
		return !(operator==(left_i, key));
	};
};