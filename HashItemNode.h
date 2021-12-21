#pragma once

#include <functional>
#include <ostream>
#include "HashItem.h"

/// <summary>
/// We store HashItemNodes for chaining in case of collisions
/// </summary>
class HashItemNode
{
private:
	HashItem* value;
	HashItemNode* next;

public:
	HashItemNode() : value(nullptr), next(nullptr) {};
	HashItemNode(HashItem* item) : value(item), next(nullptr) {};
	virtual ~HashItemNode() = default;
	HashItemNode(const HashItemNode& h);

	// todo destructor?
	HashItem* getValue() const { return value; } /*inline*/
	HashItemNode* getNext() const { return next; } /* inline*/
	void setNext(HashItemNode* next) { this->next = next; } /*  inline */
	/// <summary>
	/// Check if a HashItem exists in the linked list.
	/// </summary>
	/// <param name="predicate">Predicate to check exists condition</param>
	/// <returns></returns>
	bool exists(const std::function<bool(HashItem* item)> predicate);
	void add(HashItem* item);
	HashItem* get(type_key key) const;

	HashItemNode& operator=(const HashItemNode& other);

	friend bool operator==(const HashItemNode& left_h, const HashItemNode& right_h);

	friend bool operator!=(const HashItemNode& left_h, const HashItemNode& right_h)
	{
		return !(left_h == right_h);
	};
	friend bool operator==(const HashItemNode& left_i, const type_key& key)
	{
		return left_i.getValue()->get_key() == key;
	};

	friend bool operator!=(const HashItemNode& left_i, const type_key& key)
	{
		return !(operator==(left_i, key));
	};

	friend std::ostream& operator<<(std::ostream& os, const HashItemNode& n);
	friend void swap(HashItemNode& i1, HashItemNode& i2);
};