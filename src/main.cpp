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

#define NAMESPACE std

	NAMESPACE::map<int, const char *> sium;
	NAMESPACE::pair<NAMESPACE::map<int, const char *>::iterator, bool> ret;

	ret = sium.insert(NAMESPACE::make_pair(10, "COCAINA"));
	ret = sium.insert(NAMESPACE::make_pair(10, "COCAINAAAA"));
}
