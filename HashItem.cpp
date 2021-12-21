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