#include <testing/VecTester.hpp>
#include <testing/StackTester.hpp>
#include <testing/MapTester.hpp>
#include <rbtree.hpp>

int main() {

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

	for (int i = 0; i < 15; ++i)
		test.insert(i + i * 4);

	test.print_tree();


	/*

				10N
		2N				12R
	0R		4R		11N		20N
						 		60R

	Dopo
	 					10N
			2N						12R
	 0R				4R		11N				60
	 									20



	 */



}
