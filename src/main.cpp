#include "map.hpp"
#include <map>

int main() {
/*
	std::vector<const int> ric;

	ric.begin();

	std::cout << "========== VECTOR ==========" << std::endl;
	vec::testConstructors();
	vec::testDestructors();
	vec::testAssignOperator();
	vec::testIterators();
	vec::testCapacity();
	vec::testAccess();
	vec::testModifiers();

	std::cout << "========== STACK ==========" << std::endl;
	stack::testConstructors();
	stack::testDestructors();
	stack::testAssignOperator();
	stack::testCapacity();


	ft::RBTree<int> test;

	for (int i = 0; i < 20; ++i)
		test.insert( i + i * 4);

	test.print_tree();*/

	ft::map<int, const char *> mp;
	ft::map<int, const char *> mp2;

	mp.swap(mp2);

}
