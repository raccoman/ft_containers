#include <testing/VecTester.hpp>

int main() {

	std::cout << "========== VECTORS ==========" << std::endl;
	vec::testConstructors();
	vec::testDestructors();
	vec::testAssignOperator();
	vec::testIterators();
	vec::testCapacity();
	vec::testAccess();
	vec::testModifiers();

}
