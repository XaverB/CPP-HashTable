#include "HashTableNodeProxy.h"
#include "HashTable.h"

HashTableNodeProxy::HashTableNodeProxy(HashTable* table, type_key key) : table(table), key(key)
{
}

const type_value& HashTableNodeProxy::operator=(const type_value& value)
{
	table->put(key, value);
	return table->get(key);
}

HashTableNodeProxy::operator type_value() const
{
	return table->get(key);
}
