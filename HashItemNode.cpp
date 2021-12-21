#include "HashItemNode.h"
#include <functional>
#include "HashItem.h"

// copy constructor
HashItemNode::HashItemNode(const HashItemNode& h) : next(nullptr), value(nullptr)
{
	// ugh
	if (h != nullptr) {
		next = new HashItemNode{ *h.getNext() };
		auto item = &(*h.getValue());

		value = item == nullptr ? nullptr : new HashItem{ *h.getValue() };
	}
	else {
		next = nullptr;
		value = nullptr;
	}
}

bool HashItemNode::exists(const std::function<bool(HashItem* item)> predicate)
{
	auto node = this;
	while (node != nullptr) {
		if (predicate(node->getValue()))
			return true;
		node = node->getNext();
	}
	return false;
}

void HashItemNode::add(HashItem* item)
{
	auto node = this;
	while (node->next != nullptr) {;
		node = node->getNext();
	}

	HashItemNode* new_node = new HashItemNode{ item };
	node->setNext(new_node);
}

HashItem* HashItemNode::get(type_key key) const
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

bool operator==(const HashItemNode& left_h, const HashItemNode& right_h)
{
	auto node1 = &left_h;
	auto node2 = &right_h;

	if (node1 == nullptr && node2 == nullptr)
	{
		return true;
	}

	while (node1 != nullptr /*&& node2 != nullptr*/) {
		//if (*node1 != *node2)
		//	return false;

		auto item1 = node1->getValue();
		auto item2 = node2->getValue();
		if (item1 == nullptr && item2 != nullptr)
			return false;
		if (item1 != nullptr && item2 == nullptr)
			return false;

		if (item1 != nullptr && item2 != nullptr && *item1 != *item2)
			return false;

		node1 = node1->getNext();
		node2 = node2->getNext();
	}
	//if (node1 != node2)
	//	return false;

	return true;
}

std::ostream& operator<<(std::ostream& os, const HashItemNode& n)
{
	auto node = &n;
	while (node != nullptr) {

		auto item = node->getValue();
		os << *item;
		node = node->getNext();
	}
	return os;
}

void swap(HashItemNode& i1, HashItemNode& i2)
{
	using std::swap;
	swap(i1.next, i2.next);
	swap(i1.value, i2.value);
}

HashItemNode& HashItemNode::operator=(const HashItemNode& other)
{
	HashItemNode temp(other);
	swap(*this, temp);
	return *this;
}
