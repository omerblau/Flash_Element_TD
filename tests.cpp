// tests.cpp file
#include <iostream>
#include <cassert>
#include "bagel.h"

using namespace std;
using namespace bagel;


void test1() {
	ent_type e0 = World::createEntity();
	assert(e0.id == 0 && "First id is not 0");

	ent_type e1 = World::createEntity();
	assert(e1.id == 1 && "Second id is not 1");

	World::destroyEntity(e0);
	e0 = World::createEntity();
	assert(e0.id == 0 && "Id 0 not recycled after destroy & create");

	cout << "Test 1 passed\n";
}

void test_DynamicBag() {
	DynamicBag<int, 2> bag;

	// Initial size and capacity
	assert(bag.size() == 0);
	assert(bag.capacity() >= 2);

	// Push values
	bag.push(10);
	bag.push(20);
	assert(bag.size() == 2);
	assert(bag[0] == 10);
	assert(bag[1] == 20);

	// Trigger resize and push another
	bag.push(30);
	assert(bag.size() == 3);
	assert(bag[2] == 30);

	// Pop and check
	int last = bag.pop();
	assert(last == 30);
	assert(bag.size() == 2);

	cout << "test_DynamicBag passed\n";
}

void test_PackedStorage() {
	using IntStorage = PackedStorage<int>;

	ent_type e0{0};
	ent_type e1{1};

	// Add components
	IntStorage::add(e0, 100);
	IntStorage::add(e1, 200);

	// Get and check
	assert(IntStorage::get(e0) == 100);
	assert(IntStorage::get(e1) == 200);

	// Remove e0 and verify e1 is still correct
	IntStorage::del(e0);
	assert(IntStorage::get(e1) == 200);

	cout << "test_PackedStorage passed\n";
}

void run_tests() {
	test1();
	test_DynamicBag();
	test_PackedStorage();
}