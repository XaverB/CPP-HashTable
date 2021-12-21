#include "HashItem.h"
#include <ostream>

using std::ostream;

HashItem::HashItem(const HashItem& item)
{
	this->key = item.get_key();
	this->value = item.get_value();
}

HashItem& HashItem::operator=(const HashItem& other)
{
	HashItem temp(other);
	swap(*this, temp);
	return *this;
}

bool operator==(const HashItem& left_i, const HashItem& right_i)
{
	if (&left_i == nullptr && &right_i == nullptr)
		return true;
	if (&left_i == nullptr || &right_i == nullptr)
		return false;

	return left_i.get_key() == right_i.get_key() 
		&& left_i.get_value() == right_i.get_value();
}

std::ostream& operator<<(ostream& os, const HashItem& i)
{
	if(&i != nullptr)
		os << "[" << i.get_key() << ", " << i.get_value() << "]";
	return os;
}

void swap(HashItem& i1, HashItem& i2)
{
	using std::swap;
	swap(i1.key, i2.key);
	swap(i1.value, i2.value);
}