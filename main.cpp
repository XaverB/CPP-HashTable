#include <iostream>
#include <string>
#include <cstdio>

#include "HashTable.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;

using type_key = int;
using type_value = string;

const string red_prefix = "\x1B[31m";
const string red_postfix = "\033[0m";

void assert_equal(type_value expected, type_value actual, string caller = __builtin_FUNCTION()) {
	if (expected != actual) {
		cerr << red_prefix << "Assertion (" << caller << ") failed. Expected: (" << expected << "), Actual: (" << actual << ")" << red_postfix << endl;
	}
}

void assert_true(bool value, string caller = __builtin_FUNCTION()) {
	if (!value) {
		cerr << red_prefix << "Assert True (" << caller << ") failed." << red_postfix << endl;
	}
}

void test_put_get() {
	cout << "=== === === test put and get === === ===" << endl;
	const int size = 10;
	const string collision_postfix = "_c";
	auto default_value = type_value{};
	HashTable<type_key, type_value> t1{ size };

	cout << "1. Testing put / get without collissions " << endl;
	// no collisions
	for (int i = 1; i < size + 1; i++) {
		t1.put(i, std::to_string(i));
	}

	for (int i = 1; i < size + 1; i++) {
		auto value = t1.get(i);
		assert_equal(std::to_string(i), value);
	}

	cout << "2. Testing put / get with collissions" << endl;
	// collisions -> hashtable will use chaining internal
	for (int i = 1; i < size + 1; i++) {
		t1.put(i * 100, std::to_string(i) + collision_postfix);
	}

	for (int i = 1; i < size + 1; i++) {
		auto value = t1.get(i * 100);
		assert_equal(std::to_string(i) + collision_postfix, value);
	}

	cout << "3. Testing put / get with value_type default and overwrite existing values " << endl;
	for (int i = 1; i < size + 1; i++) {
		t1.put(i, type_value{});
		t1.put(i * 100, type_value{});
	}

	for (int i = 1; i < size + 1; i++) {
		auto value = t1.get(i);
		assert_equal(default_value, value);
		value = t1.get(i * 100);
		assert_equal(default_value, value);
	}
}

void test_clear() {
	cout << "=== === === test clear === === ===" << endl;
	const int size = 10;
	const auto default_value = type_value{};
	HashTable<type_key, type_value> t1{ size };

	cout << "1. Testing clear without collissions " << endl;
	for (int i = 1; i < size + 1; i++) {
		t1.put(i, std::to_string(i));
	}

	t1.clear();
	for (int i = 1; i < size + 1; i++) {
		auto value = t1.get(i);
		assert_equal(default_value, value);
	}

	cout << "2. Testing clear with collissions " << endl;
	for (int i = 1; i < size + 1; i++) {
		t1.put(i, std::to_string(i));
		t1.put(i * 100, std::to_string(i));
	}

	t1.clear();
	for (int i = 1; i < size + 1; i++) {
		auto value = t1.get(i);
		assert_equal(default_value, value);
		value = t1.get(i * 100);
		assert_equal(default_value, value);
	}
}


void test_contains() {
	cout << "=== === === test contains === === ===" << endl;
	const int size = 10;
	const auto default_value = type_value{};
	HashTable<type_key, type_value> t1{ size };

	cout << "1. Testing contains without collissions " << endl;
	for (int i = 1; i < size + 1; i++) {
		t1.put(i, std::to_string(i));
	}

	for (int i = 1; i < size + 1; i++) {
		auto contains_value = t1.contains(i);
		assert_true(contains_value);
	}

	cout << "2. Testing contains with collissions" << endl;
	for (int i = 1; i < size + 1; i++) {
		t1.put(i * 100, std::to_string(i));
	}

	for (int i = 1; i < size + 1; i++) {
		auto contains_value = t1.contains(i * 100);
		assert_true(contains_value);

		contains_value = t1.contains(i * 1);
		assert_true(contains_value);
	}


	cout << "3. Testing contains not" << endl;
	assert_true(!t1.contains(INT_MAX));
	assert_true(!t1.contains(INT_MIN));
}

void test_contains_value() {
	cout << "=== === === test contains value === === ===" << endl;
	const int size = 10;
	const auto default_value = type_value{};
	HashTable<type_key, type_value> t1{ size };

	cout << "1. Testing contains value without collissions " << endl;
	for (int i = 1; i < size + 1; i++) {
		t1.put(i, std::to_string(i));
	}

	for (int i = 1; i < size + 1; i++) {
		auto contains_value = t1.contains_value(std::to_string(i));
		assert_true(contains_value);
	}

	cout << "2. Testing contains value with collissions" << endl;
	for (int i = 1; i < size + 1; i++) {
		t1.put(i * 100, std::to_string(i * 100));
	}

	for (int i = 1; i < size + 1; i++) {
		auto contains_value = t1.contains_value(std::to_string(i * 100));
		assert_true(contains_value);

		contains_value = t1.contains_value(std::to_string(i));
		assert_true(contains_value);
	}


	cout << "3. Testing contains not" << endl;
	assert_true(!t1.contains_value("i am not existend in the hashtable"));
	assert_true(!t1.contains_value("me neither"));
}

void test_print() {
	cout << "=== === === test print === === ===" << endl;
	const int size = 10;
	const auto default_value = type_value{};
	HashTable<type_key, type_value> t1{ size };

	cout << "1. Testing print without collissions " << endl;
	for (int i = 1; i < size + 1; i++) {
		t1.put(i, std::to_string(i));
	}
	cout << t1 << endl;

	cout << "2. Testing print with with collissions" << endl;
	for (int i = 1; i < size + 1; i++) {
		t1.put(i * 100, std::to_string(i * 100));
	}

	cout << t1 << endl;
}

void test_assignment_copy_move() {
	cout << "=== === === test assignment copy move === === ===" << endl;
	const int size = 10;
	const auto default_value = type_value{};
	HashTable<type_key, type_value> t1{ size };

	cout << "1. Testing asignment without collissions " << endl;
	for (int i = 1; i < size + 1; i++) {
		t1.put(i, std::to_string(i));
	}
	HashTable<type_key, type_value> t2 = t1;
	assert_true(t1 == t2);
	cout << t1 << endl;
	cout << t2 << endl;

	cout << "2. Testing print with with collissions" << endl;
	for (int i = 1; i < size + 1; i++) {
		t1.put(i * 100, std::to_string(i * 100));
	}

	HashTable<type_key, type_value> t3 = t1;
	assert_true(t1 == t3);
	cout << t1 << endl;
	cout << t3 << endl;

	cout << "3. Testing copy without collissions " << endl;
	HashTable<type_key, type_value> t4(t2);
	assert_true(t4 == t2);
	cout << t2 << endl;
	cout << t4 << endl;

	cout << "4. Testing copy with collissions" << endl;
	HashTable<type_key, type_value> t5{ t4 };
	assert_true(t4 == t5);
	cout << t4 << endl;
	cout << t5 << endl;

	cout << "5. Testing move without collissions" << endl;
	HashTable<type_key, type_value> t6(std::move(t2));
	assert_true(t6 == t4);
	cout << t6 << endl;
	cout << t4 << endl;

	// t2 should be in a bad state after move
	assert_true(t6 != t2);

	cout << "6. Testing move with collissions" << endl;
	HashTable<type_key, type_value> t7(std::move(t5));
	assert_true(t7 == t4);
	cout << t4 << endl;
	cout << t7 << endl;

	assert_true(t7 != t5);
}

void test_index_operator() {
	cout << "=== === === test index operator === === ===" << endl;
	const int size = 10;
	const auto default_value = type_value{};
	HashTable<type_key, type_value> t1{ size };

	cout << "1. Testing index operator without collission " << endl;
	for (int i = 1; i < size + 1; i++) {
		t1[i] = std::to_string(i);
	}

	for (int i = 1; i < size + 1; i++) {
		auto value = t1[i];
		assert_equal(std::to_string(i), value);
	}

	cout << "2. Testing index operator with collission " << endl;
	for (int i = 1; i < size + 1; i++) {
		t1[i * 100] = std::to_string(i * 100);
	}

	for (int i = 1; i < size + 1; i++) {
		auto value = t1[i * 100];
		assert_equal(std::to_string(i * 100), value);
	}
}

void run_tests() {
	test_put_get();
	cout << endl;

	test_clear();
	cout << endl;

	test_contains();
	cout << endl;

	test_contains_value();
	cout << endl;

	test_print();
	cout << endl;

	test_assignment_copy_move();
	cout << endl;

	test_index_operator();
}

int main() {
	cout << "=== === === Test runner started === === ===" << endl;
	run_tests();
	cout << endl;

	return 0;
}
